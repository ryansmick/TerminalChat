// TCP Client

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../message.h"
#include "../tcp_connection.h"
#include "client.h"
#include <pthread.h>

#define BUFF_SIZE 4096

using namespace std;

int main(int argc, char **argv) {
	
	string text = "hello ## wo#rld###";
	cout << text << endl;

	Message message = Message(text, true, false);
	Message new_message = Message::decode(message.encode());
	cout << new_message.get_message_text() << endl;
	cout << new_message.get_is_command() << endl;
	cout << new_message.get_is_prompted() << endl;
	
	if(argc != 4) {
		fprintf(stderr, "usage: tcpclient server_hostname server_port username\n");
		exit(1);
	}

	char *hostname = argv[1];
	char *port = argv[2];
	//char *username = argv[3];

	char *buf = (char *)malloc(BUFF_SIZE);
	bzero(buf,BUFF_SIZE);

	Client c = Client(hostname, port);
	
	//client.connect_socket();
	c.start();
	//client.close_socket();
	
	return 0;
}
