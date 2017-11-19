#include <cstdio>
#include <cstdlib>
#include <netdb.h>
#include <strings.h>
#include <unistd.h>

#include "tcp_server.h"
#include "../tcp_connection.h"

using namespace std;

void TCPServer::open_socket(int &sockfd) {

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket() failed");
		exit(1);
	}

}

void TCPServer::server_bind_socket() {

	// allow for port reuse
	int opt = 0;
	setsockopt(this->connection_socket, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(int));

	// bind socket
	if((bind(this->connection_socket, (struct sockaddr *) &this->server_addr, sizeof(this->server_addr))) < 0) {
		perror("bind() failed");
		close(this->connection_socket);
		exit(1);
	}
};

void TCPServer::server_listen_socket() {
	
	if(listen(this->connection_socket, 1) == -1) {
		perror("listen() failed");
		close(this->connection_socket);
		exit(1);
	}
}

TCPServer::TCPServer() {}

TCPConnection& TCPServer::accept_connection() {
	
	int data_socket;
	if((data_socket = accept(this->connection_socket, (struct sockaddr *) &this->client_addr, &this->client_addr_size)) == -1) {
		perror("accept() failed");
		close(this->connection_socket);
		exit(1);
	}

	TCPConnection* conn = new TCPConnection(data_socket);
	return *conn;
}

void TCPServer::start_server(char *port) {

	// create server address struct
	bzero((char *) &this->server_addr, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(atoi(port));
	this->client_addr_size = sizeof(this->client_addr);

	open_socket(this->connection_socket);
	server_bind_socket();
	server_listen_socket();
}

