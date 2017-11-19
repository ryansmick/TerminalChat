
#include <cstdio>
#include <cstdlib>
#include <netdb.h>
#include <strings.h>
#include <unistd.h>

#include "tcp_connection.h"
using namespace std;

// Convert a c string into a cpp string and return the cpp string
string TCPConnection::c_to_cpp_string(char* buf) {

	string str;
	if(buf == NULL) {
		str = string("");
	}
	else {
		str = string(buf);
	}
	return str;
}

string TCPConnection::rstrip(string str) {
	return str.substr(0, str.find_last_not_of(" \t\n") + 1);
}

void TCPConnection::open_socket(int &sockfd) {
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket() failed");
		exit(1);
	}
};

TCPConnection::TCPConnection() {}

void TCPConnection::client_connect_socket() {
	
	if(connect(this->client_socket, (struct sockaddr *) &this->client_addr, sizeof(this->client_addr)) < 0) {
		perror("tcp-client: connect() failed");	
		close(this->client_socket);
		exit(1);
	}
};

void TCPConnection::server_bind_socket() {

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

void TCPConnection::server_listen_socket() {
	
	if(listen(this->connection_socket, 1) == -1) {
		perror("listen() failed");
		close(this->connection_socket);
		exit(1);
	}
}

void TCPConnection::server_accept_connection() {

	if((this->data_socket = accept(this->connection_socket, (struct sockaddr *) &this->client_addr, &this->client_addr_size)) == -1) {
		perror("accept() failed");
		close(this->connection_socket);
		exit(1);
	}
}

void TCPConnection::start_client(char *host, char *port) {

	// create the hostent structure
	this->hp = gethostbyname(host);	
	if(!this->hp) {	
		fprintf(stderr, "tcp-client: unknown host: %s\n", host);	
		exit(1);
	}	

	// create client address struct
	bzero((char	*)&this->client_addr, sizeof(struct sockaddr_in));	
	this->client_addr.sin_family = AF_INET;	
	bcopy(this->hp->h_addr, (char *)&this->client_addr.sin_addr, this->hp->h_length);	
	this->client_addr.sin_port = htons(atoi(port));

	open_socket(client_socket);
	client_connect_socket();
}

void TCPConnection::start_server(char *port) {

	// create server address struct
	bzero((char *) &this->server_addr, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(atoi(port));
	this->client_addr_size = sizeof(this->client_addr);

	open_socket(this->connection_socket);
	server_bind_socket();
	server_listen_socket();
	server_accept_connection();
}
