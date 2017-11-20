// Member functions for the Chatserver

#include <cstdio>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <unordered_map>
#include "chatserver.h"
using namespace std;

typedef struct thread_data_t {
	Chatserver *server;
	TCPConnection *conn;
} ThreadData;

Chatserver::Chatserver(char *port) {

	// Start server
	this->tcp_server = TCPServer();
	this->tcp_server.start_server(port);
	
	// Load user login information
	string line, username, password;
	ifstream f("passwords.txt");
	if(f.is_open()) {
		while(getline(f, line)) {
			split(line, username, password);
			this->passwords.insert(pair<string, string>(username, password));
		}
	}
}

void Chatserver::accept_connections() {

	while(true) {
		ThreadData thread_data = ThreadData{this, &this->tcp_server.accept_connection()};
		pthread_t thread_id;
		if((pthread_create( &thread_id, NULL, &Chatserver::client_handler, (void*) &thread_data) < 0)) {
			perror("Failed to create thread");
			continue;
		}
		this->threads.push_back(thread_id);
	}
}

void *Chatserver::client_handler(void *data) {
	
	// Retrieve username
	ThreadData *thread_data = (ThreadData *) data;
	Chatserver *server = thread_data->server;
	TCPConnection *conn = thread_data->conn;
	Message m = server->wait_for_ack(*conn);
	string username = m.get_message_text();
	
	// Check if user has logged in before
	unordered_map<string, string>::const_iterator it = server->passwords.find(username);
	if(it == server->passwords.end()) {
		// Username not found
		m = Message("create", false, true);
		conn->send_message(m);
		
		m = server->wait_for_ack(*conn);
		string password = m.get_message_text();

		ofstream myfile;
		myfile.open("passwords.txt", ofstream::app);
		myfile << username << " " << password << "\n";
		myfile.close();
		m = Message("finished", false, true);
		conn->send_message(m);
	}
	else {
		// Username found
		Message m = Message("login", false, true);
		conn->send_message(m);
		m = server->wait_for_ack(*conn);
		while(m.get_message_text() != it->second) {
			conn->send_message(Message("login failed", false, true));
			m = server->wait_for_ack(*conn);
		}
		m = Message("logged in", false, true);
		conn->send_message(m);
		server->connections.insert(pair<string, TCPConnection>(username, *conn));
	}

	while(true) {
		// Wait for client command
		m = server->wait_for_command(*conn);

		// Parse command
		string command = m.get_message_text();
		cout << "INCOMING COMMAND: " << command << endl;
		if(command == "B") {
			conn->send_message(Message("send message", false, true));
			cout << "SENT BROADCASE RESPONSE" << endl;
			m = server->wait_for_ack(*conn);
			cout << "SERVER WAITED FOR ACK" << endl;
			m.set_is_prompted(false);
			server->broadcast(m, username);
			cout << "THE MESSAGE WAS BROADCAST" << endl;
			conn->send_message(Message("Your message was sent to all other logged-in users.", false, true));
		}
		else if(command == "P") {
			string user_list = server->list_online_users();
			conn->send_message(Message(user_list, false, true));
			m = server->wait_for_ack(*conn);
			string target = m.get_message_text();
			m = server->wait_for_ack(*conn);
			m.set_is_prompted(false);
			if(server->private_message(m, username, target)) {
				conn->send_message(Message("Your message was succeccfully sent to " + target + ".", false, true));
			}
			else {
				conn->send_message(Message(target + " is offline.", false, true));
			}
		}
		else if(command == "E") {
			conn->close_socket();
			server->connections.erase(username);
			return 0;
		}
	}

	return 0;
}

void Chatserver::broadcast(Message m, string username) {

	for(auto it = this->connections.begin(); it != this->connections.end(); ++it) {
		if(username != it->first) {
			it->second.send_message(m);
		}
	}
}

bool Chatserver::private_message(Message m, string sender, string receiver) {

	m.set_message_text("(" + sender + ") " + m.get_message_text());
	auto it = this->connections.find(receiver);
	if(it == this->connections.end()) {
		return false;
	}
	else {
		it->second.send_message(m);
		return true;
	}
}

string Chatserver::list_online_users() {

	string user_list = "Online users:\n";
	for(auto it = this->connections.begin(); it != this->connections.end(); ++it) {
		user_list += ("\t" + it->first + "\n");
	}
	return user_list;
}

// Remove whitespace from the end of a string
string Chatserver::rstrip(string str) {
	return str.substr(0, str.find_last_not_of(" \t\n") + 1);
}

// Split a single string into two strings at the first space
void Chatserver::split(string line, string &s1, string &s2) {

	string::size_type pos;
	pos=line.find(' ',0);
	if(pos != string::npos) {
		s1 = line.substr(0,pos);
		s2 = line.substr(pos+1);
	}
}

Message Chatserver::wait_for_ack(TCPConnection conn) {
	while(1) {
		if(conn.is_message_available()) {
			Message m = conn.get_latest_message();
			if(m.get_is_prompted()) {
				conn.pop_latest_message();
				return m;
			}
		}
	}	
}

Message Chatserver::wait_for_command(TCPConnection conn) {
	while(1) {
		if(conn.is_message_available()) {
			Message m = conn.get_latest_message();
			if(m.get_is_command()) {
				conn.pop_latest_message();
				return m;
			}
		}
	}
}
