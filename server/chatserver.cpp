// Member functions for the Chatserver

#include <cstdio>
#include <fstream>
#include <iostream>
#include <pthread.h>

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
	while(!conn->is_message_available()) {}
	Message message = conn->get_latest_message();
	conn->pop_latest_message();
	string username = message.get_message_text();
	cout << username << " connected!" << endl;
//	delete &message;

	// Check if user has logged in before
	unordered_map<string, string>::const_iterator it = server->passwords.find(username);
	if(it == server->passwords.end()) {
		// Username not found
		cout << "Username NOT found" << endl;
//		Message m = Message("create", false, true);
//		conn->send_message(m);
	}
	else {
		// Username found
		cout << "Username found!" << endl;
//		Message m = Message("login", false, true);
//		conn->send_message(m);
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
