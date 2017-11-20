// TCP Client Class Definition
#ifndef CLIENT
#define CLIENT
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string>
#include "../tcp_connection.h"
using namespace std;

class Client {

	private:
		TCPConnection tcp_connection;	
		pthread_t handle_messages_thread;

		void broadcast_message();
		void private_message();
		void user_logout();
		void user_login(char *username);
		static void *handle_unprompted_messages(void *arg);
		
		Message wait_for_ack();

		string strip(string s);


	public:

		Client(char *h, char *port, char *username);
		
		void start();	
		void print_usage();
		
		void open_socket();
		void connect_socket();
		void send_message(string s);
		string receive_data();
	
		void close_socket();
};

#endif
