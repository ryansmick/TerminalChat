
#include <cstdio>
#include <cstdlib>
#include <netdb.h>
#include <strings.h>
#include <unistd.h>

#include "tcp_connection.h"
#include "message.h"

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

TCPConnection::TCPConnection() {
	this->is_connected = false;
}

TCPConnection::TCPConnection(int data_socket) {

	this->data_socket = data_socket;
	this->is_connected = true;

}

void TCPConnection::client_connect_socket() {
	
	if(connect(this->data_socket, (struct sockaddr *) &this->client_addr, sizeof(this->client_addr)) < 0) {
		perror("tcp-client: connect() failed");	
		close(this->data_socket);
		exit(1);
	}
};

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

	open_socket(data_socket);
	client_connect_socket();

	this->is_connected = true;
}

void TCPConnection::send_message(Message message) {

	// Encode the object to be sent
	string encoded_message = message.encode();

	// Encode the newly encoded string to escape the message separation delimiter
	string separated_message = encode_message(encoded_message);

	// Send the message across the socket
	send_message(separated_message);

}

void TCPConnection::send_message(string s) {
	int len = s.length();
	if(send(this->data_socket, s.c_str(), len, 0) == -1) {
		fprintf(stderr, "tcp-client: send() failed");
		close_socket();
		exit(1);
	}
}


string TCPConnection::encode_message(string message_string) {

	return message_string;

}

void TCPConnection::close_socket() {

	close(this->data_socket);

}


void TCPConnection::send_message(Message m) {
}
