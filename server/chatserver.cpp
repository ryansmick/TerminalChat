// Member functions for the Chatserver

#include <cstdio>
#include <iostream>
#include <pthread.h>

#include "chatserver.h"
using namespace std;

Chatserver::Chatserver(char *port) {

	this->tcp_server = TCPServer();
	this->tcp_server.start_server(port);
}

void Chatserver::accept_connections() {

	while(true) {
		TCPConnection conn = this->tcp_server.accept_connection();
		pthread_t thread_id;
		if((pthread_create( &thread_id, NULL, &this->client_handler, (void*) &conn) < 0)) {
			perror("Failed to create thread");
			continue;
		}
		this->threads.push_back(thread_id);
	}
}

void *client_handler(void *data) {
	TCPConnection *conn = (TCPConnection *) data;
	while(!conn->is_message_available()) {}
	Message message = conn->get_latest_message();
	conn->pop_latest_message();
	string name = message.get_message_text();
	cout << name << " connected!" << endl;
	delete &message;
}

