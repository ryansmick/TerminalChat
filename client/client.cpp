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

bool end_child_thread = false;

void* Client::handle_unprompted_messages(void *arg) {
	Client *c = (Client *)arg;
	while(!end_child_thread) {
		if(c->tcp_connection.is_message_available()) {
			Message m = c->tcp_connection.get_latest_message();
			if(!m.get_is_prompted()) {
				c->tcp_connection.pop_latest_message();
			}
			string text = m.get_message_text();
			cout << "**** NEW MESSAGE: " << text << " *****************" << endl;
		}	
	}
	pthread_exit(NULL);
}

Client::Client(char *h, char *port, char *username) {
	tcp_connection = TCPConnection();
	tcp_connection.start_client(h, port);
	this->user_login(username);
};

void Client::start() {	
	int ret =  pthread_create(&handle_messages_thread, NULL, &handle_unprompted_messages, (void *)this);
	if(ret != 0) {
		printf("Error: pthread_create() failed\n");
		exit(EXIT_FAILURE);														                
	}

	printf("Welcome to the Online Chat Room!\n");
	this->print_usage();

	cout << endl;
	cout << "> ";
	string command;
	while(cin >> command) { // loop until the input is finished
		if(command == "E") {
			printf("Goodbye!\n");
			user_logout();
			end_child_thread = 1;
			pthread_join(handle_messages_thread, NULL);
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
	printf("OPERATIONS:\n");
	printf("\tP -- send a private chat message\n");
	printf("\tB -- broadcast a chat message to all users\n");
	printf("\tE -- exit this chat client\n");
}

void Client::broadcast_message() {
	Message m = Message("B", true, false);
	tcp_connection.send_message(m);

	m = wait_for_ack();
	
	string text = m.get_message_text();
	if(text == "send message") {
		string text;
		cout << "What is the message you want to broadcast?" << endl << "\t>";
		cin >> text;
		m = Message(text, false, true);
		tcp_connection.send_message(m);
	}
}

void Client::private_message() {
	Message m = Message("P", true, false);
	tcp_connection.send_message(m);

	m = wait_for_ack();
	
	string text = m.get_message_text();
	if(text == "send message") {
		string text;
		cout << "What is the message you want to send?" << endl << "\t>";
		cin >> text;
		m = Message(text, false, true);
	}
}

void Client::user_logout() {
	Message m = Message("E", true, false);
}

void Client::user_login(char *username) {
	if(!username) {
		fprintf(stderr, "unable to log in with no username\n");
	}

	Message m = Message(string(username), false, true);
	this->tcp_connection.send_message(m);

	m = wait_for_ack();
	
	string text = m.get_message_text();
	if(text == "login") {
		string password;
		cout << "Account found! Please enter your password: ";
		cin >> password;
		while(1) {
			//delete(&m);
			m = Message(password, false, true);
			tcp_connection.send_message(m);
			m = wait_for_ack();
			if(m.get_message_text() == "logged in") {
				break;
			}
			cout << "Incorrect Password. Please re-enter your password: ";
			cin >> password;
		}
	} else if (text == "create") {
		string password;
		cout << "No Account found with that username. Please create a password: ";
		cin >> password;
		cout << "password: " << password << endl;
		//delete(&m);
		m = Message(password, false, true);
		tcp_connection.send_message(m);
		m = wait_for_ack();	
	}
	//delete(&m);
}

Message Client::wait_for_ack() {
	while(1) {
		if(tcp_connection.is_message_available()) {
			Message m = tcp_connection.get_latest_message();
			if(m.get_is_prompted()) {
				tcp_connection.pop_latest_message();
				return m;
			}
		}
	}	
}
