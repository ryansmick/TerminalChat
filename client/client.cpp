// TCP Client Function Implementation
// Ryan Smick
// Connor Higgins
// Cameron Smick

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>	
#include <sys/types.h>	
#include <sys/socket.h>	
#include <netinet/in.h>	
#include <netdb.h>	
#include <unistd.h>
#include <string>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>

#include "client.h"
#include "../tcp_connection.h"
using namespace std;

void* Client::handle_unprompted_messages(void *arg) {

	//while(1) {
	//}
	printf("in a new thread\n");
	pthread_exit(NULL);
}

Client::Client(char *h, char *port) {
	tcp_connection = TCPConnection();
	tcp_connection.start_client(h, port);
};

void Client::start() {	
	int ret =  pthread_create(&handle_messages_thread, NULL, &handle_unprompted_messages, NULL);
	if(ret != 0) {
		printf("Error: pthread_create() failed\n");
		exit(EXIT_FAILURE);														                
	}

	printf("Welcome to the Online Chat Room!\n");
	this->print_usage();

	cout << endl << endl;
	cout << "> ";
	string command;
	while(cin >> command) { // loop until the input is finished
		if(command == "E") {
			printf("Goodbye!\n");
			user_logout();
			break;
		} else if(command == "B"){
			broadcast_message();
		} else if(command == "P"){
			private_message();
		} else {
			cout << "INVALID COMMAND\n\n";
		}

		cout << endl << endl;

		this->print_usage();

		cout << endl << endl;	
		cout << "> ";
		command.clear();
	}
};

void Client::print_usage() {
}

void Client::broadcast_message() {
	Message m = Message("B", true, false);
	tcp_connection.send_message(m);

	
}

void Client::private_message() {
}

void Client::user_logout() {
}

void Client::user_login() {
}
