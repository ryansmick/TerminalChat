// Chatserver class definition

#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <unordered_map>
#include <pthread.h>
#include <vector>

#include "tcp_server.h"

class Chatserver {

	private:

		// Data members
		TCPServer tcp_server;
		std::vector<pthread_t> threads;
		unordered_map<string, TCPConnection> connections;
		unordered_map<string, string> passwords;

		// Utility functions
		string rstrip(string str);
		void split(string line, string &s1, string &s2);

		// Private member functions
		static void *client_handler(void *data);

	public:

		// Constructor
		Chatserver(char *port);

		// Member functions
		void accept_connections();
};

#endif
