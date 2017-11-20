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
	if(argc != 4) {
		fprintf(stderr, "usage: tcpclient server_hostname server_port username\n");
		exit(1);
	}

	char *hostname = argv[1];
	char *port = argv[2];
	char *username = argv[3];

	char *buf = (char *)malloc(BUFF_SIZE);
	bzero(buf,BUFF_SIZE);

	Client c = Client(hostname, port, username);
	
	//client.connect_socket();
	c.start();
	//client.close_socket();
	
	return 0;
}
