// Chatserver class definition

#ifndef CHATSERVER_H
#define CHATSERVER_H

//#include <unordered_map>
#include <vector>

#include "tcp_server.h"

class Chatserver {

	private:

		// Data members
		TCPServer tcp_server;
		std::vector<pthread_t> threads;
//		unordered_map<string, TCPConection> connections;

		// Private member functions
		static void *client_handler(void *data);

	public:

		// Constructor
		Chatserver(char *port);

		// Member functions
		void accept_connections();
};

#endif
