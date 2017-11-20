#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <netdb.h>
#include <strings.h>
#include <unistd.h>
#include <queue>
#include <vector>
#include <fcntl.h>
#include <errno.h>
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
	this->message_queue = queue<Message>();
	this->is_connected = false;
}

TCPConnection::TCPConnection(int data_socket) {
	
	this->message_queue = queue<Message>();
	this->data_socket = data_socket;
	fcntl(this->data_socket, F_SETFL, O_NONBLOCK);
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

	fcntl(this->data_socket, F_SETFL, O_NONBLOCK);

	this->is_connected = true;
}

void TCPConnection::send_message(Message message) {

	// Encode the object to be sent
	string encoded_message = message.encode();

	// Encode the newly encoded string to escape the message separation delimiter
	string separated_message = encode_message(encoded_message);

	separated_message += message_delimiter;

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

	string encoded_string = string(message_string);

	// Escape the delimiter
	for(size_t i = 0; i < encoded_string.length(); i++) {
		if (encoded_string.at(i) == this->message_delimiter) {
			encoded_string.insert(i, 1, this->message_delimiter);
			i++;
		}
	}

	return encoded_string;

}

string TCPConnection::decode_message(string encoded_string) {

	string text = string(encoded_string);

	// Remove the extra delimiters from the string
	for(size_t i = 0; i < text.length()-1; i++) {
		if(text.at(i) == message_delimiter && text.at(i+1) == message_delimiter) {
			text.erase(i+1, 1);
		}
	}

	return text;

}

void TCPConnection::populate_message_queue() {

	// Retrieve latest data from socket
	string incoming_messages = receive_from_socket();

	if(incoming_messages.length() <= 0) {
		return;
	}

	cout << "Incoming messages: " << incoming_messages << endl;

	// Split messages up
	vector<string> encoded_messages;
	split_messages(incoming_messages, encoded_messages);

	// Iterate through vector adding messages to queue
	for(auto it = encoded_messages.begin(); it != encoded_messages.end(); it++) {
		string message_text = *it;
		cout << "Message text: " << message_text << endl;
		string decoded_message = decode_message(message_text);
		Message message = Message::decode(decoded_message);
		this->message_queue.push(message);
	}
}

string TCPConnection::receive_from_socket() {

	size_t in_buffer_size = 8192;
	char in_buffer[in_buffer_size];
	bzero(in_buffer, in_buffer_size);
	if(read(this->data_socket, (void *) &in_buffer, in_buffer_size) == -1 && errno != EAGAIN) {
		perror("read() failed");
		exit(1);
	}
	return rstrip(c_to_cpp_string(in_buffer));

}

void TCPConnection::split_messages(string incoming_messages, vector<string>& messages) {

	size_t i;
	size_t start_index = 0;
	for(i = 0; i < incoming_messages.length()-1; i++) {
		if (incoming_messages.at(i) == message_delimiter) {
			if (incoming_messages.at(i+1) != message_delimiter) {
				messages.push_back(incoming_messages.substr(start_index, i));
				start_index = i+1;
			}
			else {
				i++;
			}
		}
	}

	messages.push_back(incoming_messages.substr(start_index, incoming_messages.length()-1));

}

bool TCPConnection::is_message_available() {

	// Fill queue with most recent messages
	populate_message_queue();

	return !message_queue.empty();

}

Message& TCPConnection::get_latest_message() {

	populate_message_queue();

	return message_queue.front();

}

void TCPConnection::pop_latest_message() {

	populate_message_queue();

	message_queue.pop();

}

void TCPConnection::close_socket() {

		close(this->data_socket);

}
