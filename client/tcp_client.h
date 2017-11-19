// TCP Connection Class Definition

#include <netinet/in.h>
#include <sys/socket.h>
#include <string>

using namespace std;


// Struct to represent a message sent over the tcp connection
struct Message {
	string message_text; // The body of the message
	bool is_command; // Is the message a command or data
	bool is_prompted; // Whether or not the message was received in response to a previous message
};


// Class to represent the tcp connection
class TCPConnection {

	private:
		
		char *buf;
		int buf_size;
		int sockfd;
		char *host;
		struct hostent *hp;
		struct sockaddr_in sin;
		socklen_t client_addr_size;
		
		// String utility functions
		string c_to_cpp_string(char* buf);
		string rstrip(string str);

		// Open a socket on client or server
		void open_socket();

		// Client connection utility functions
		void client_connect_socket();
		
		// Server connection utility functions
		void server_bind_socket()
		void server_listen_socket();
		void server_accept_connection();

	public:

		// Constructor
		TCPConnection();

		// Set up the connection
		void start_client();
		void start_server();
	
		// Public functions to send and receive data
		void send_message(Message message);
		bool is_message_available();
		Message* get_latest_message();
		void pop_latest_message();

		// Close the connection
		void close_socket();
};
