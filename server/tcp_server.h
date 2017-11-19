// TCP server class definition

#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <string>

#include "../tcp_connection.h"

using namespace std;

class TCPServer {

	private:

		// server data members
		int connection_socket;
		int data_socket;
		struct sockaddr_in server_addr;
		struct sockaddr_in client_addr;
		socklen_t client_addr_size;

		// Open a socket on the server
		void open_socket(int &sockfd);

		// Server utility functions
		void server_bind_socket();		
		void server_listen_socket();

	public:

		// Constructor
		TCPServer();

		// Set up connection
		void start_server(char* port);
		TCPConnection& accept_connection();
		
};

#endif
