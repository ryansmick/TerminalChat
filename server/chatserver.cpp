// Member functions for the Chatserver

#include <cstdio>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <unordered_map>
#include "chatserver.h"
using namespace std;

Message wait_for_ack(TCPConnection conn);

struct Thread_Args {
	TCPConnection connection;
	unordered_map<string,string> passwords;
};

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
		struct Thread_Args args;
		args.connection = this->tcp_server.accept_connection();
		args.passwords = this->passwords;
		cout << "1 size: " << args.passwords.size() << endl;
		for(unordered_map<string, string>::iterator it = args.passwords.begin(); it != args.passwords.end(); it++) {
			cout << "1: first: " << it->first << ", second: " << it->second << endl;
		}
		//TCPConnection conn = this->tcp_server.accept_connection();
		pthread_t thread_id;
		if((pthread_create( &thread_id, NULL, &this->client_handler, (void*) &args) < 0)) {
			perror("Failed to create thread");
			continue;
		}
		this->threads.push_back(thread_id);
	}
}

void *Chatserver::client_handler(void *data) {
	
	// Retrieve username
	/*
	TCPConnection *conn = (TCPConnection *) data;
	while(!conn->is_message_available()) {}
	Message message = conn->get_latest_message();
	conn->pop_latest_message();
	string username = message.get_message_text();
	cout << username << " connected!" << endl;
	unordered_map<string, string>::const_iterator it = passwords.find(username);
	if(it == passwords.end()) {
		// Username not found
		Message m = Message("create", false, true);
		conn->send_message(m);
	}
	else {
		// Username found
		Message m = Message("login", false, true);
		conn->send_message(m);
	}
	*/
	
	struct Thread_Args *args = (struct Thread_Args*)data;
	cout << "2 size: " << args->passwords.size() << endl;
	for(unordered_map<string, string>::iterator it = (*args).passwords.begin(); it != (*args).passwords.end(); it++) {
		cout << "2: first: " << it->first << ", second: " << it->second << endl;
	}
	Message message = wait_for_ack(args->connection);
	string username = message.get_message_text();
	cout << username << " connected!" << endl;

	// Check if user has logged in before
	unordered_map<string, string>::const_iterator it = args->passwords.find(username);
	cout << "here" <<endl;
	for(unordered_map<string, string>::iterator it = args->passwords.begin(); it != args->passwords.end(); ++it) {
		cout << "first: " << it->first << ", second: " << it->second << endl;
	}
	if(it == args->passwords.end()) {
		// Username not found
		cout << "acct not found" << endl;
		Message m = Message("create", false, true);
		args->connection.send_message(m);
		
		m = wait_for_ack(args->connection);
		string password = m.get_message_text();
		cout << "password received: " << password << endl;

		ofstream myfile;
		myfile.open("passwords.txt", ofstream::app);
		myfile << username << " " << password << "\n";
		myfile.close();
		m = Message("finished", false, true);
	}
	else {
		// Username found
		Message m = Message("login", false, true);
		args->connection.send_message(m);
	}

	return 0;
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

Message wait_for_ack(TCPConnection conn) {
	cout << "waiting for ack" << endl;
	while(1) {
		if(conn.is_message_available()) {
			Message m = conn.get_latest_message();
			cout << "m: " << m.get_message_text() << endl;
			if(m.get_is_prompted()) {
				cout << "popping message with text: " << m.get_message_text() << endl;
				conn.pop_latest_message();
				if(conn.is_message_available()) {
					Message m2 = conn.get_latest_message();
					cout << "m2: " << m2.get_message_text() << endl;
				}
				return m;
			}
		}
	}	
}
