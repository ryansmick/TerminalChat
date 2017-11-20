// TCP Connection Class Definition

#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <string>
#include <queue>
#include <vector>

#include "message.h"

using namespace std;

// Class to represent the tcp connection
class TCPConnection {

	private:
		
		char *buf;
		int buf_size;

		// connection data members
		int data_socket;
		char *host;
		struct hostent *hp;
		struct sockaddr_in client_addr;
		bool is_connected;

		// Message queue
		queue<Message> message_queue;

		// String utility functions
		string c_to_cpp_string(char* buf);
		string rstrip(string str);

		// Open a socket on client or server
		void open_socket(int &sockfd);

		// Client connection utility functions
		void client_connect_socket();

		// Delimiter for messages
		const static char message_delimiter = '|';
		
		// Send and receive utility functions
		void send_message(string s);
		string encode_message(string message_string);
		string decode_message(string encoded_string);
		void populate_message_queue();
		string receive_from_socket();
		void split_messages(string incoming_messages, vector<string>& messages);

	public:

		// Constructor
		TCPConnection();
		TCPConnection(int data_socket);

		// Set up the connection
		void start_client(char *host, char *port);
	
		// Public functions to send and receive data
		void send_message(Message message);
		bool is_message_available();
		Message& get_latest_message();
		void pop_latest_message();

		// Close the connection
		void close_socket();
};

#endif
