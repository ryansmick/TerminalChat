// TCP Connection Class Definition

#include <netinet/in.h>
#include <sys/socket.h>
#include <string>

using namespace std;

// Class to represent the tcp connection
class TCPConnection {

	private:
		
		char *buf;
		int buf_size;

		// client data members
		int client_socket;
		char *host;
		struct hostent *hp;
		struct sockaddr_in client_addr;
		socklen_t client_addr_size;

		// server data members
		int connection_socket;
		int data_socket;
		struct sockaddr_in server_addr;
		
		// String utility functions
		string c_to_cpp_string(char* buf);
		string rstrip(string str);

		// Open a socket on client or server
		void open_socket(int &sockfd);

		// Client connection utility functions
		void client_connect_socket();
		
		// Server connection utility functions
		void server_bind_socket();
		void server_listen_socket();
		void server_accept_connection();

	public:

		// Constructor
		TCPConnection();

		// Set up the connection
		void start_client(char *host, char *port);
		void start_server(char *port);
	
		// Public functions to send and receive data
		void send_message(Message message);
		bool is_message_available();
		Message* get_latest_message();
		void pop_latest_message();

		// Close the connection
		void close_socket();
};
