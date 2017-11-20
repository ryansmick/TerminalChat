
#include <cstdlib>
#include <iostream>

#include "chatserver.h"
using namespace std;

int main(int argc, char **argv) {

	if(argc != 2) {
		printf("\nError: Invalid arguments\nProper command format: ./chatserver <port>\n\n");
		exit(1);
	}
	char *port = argv[1];

	Chatserver server(port);
	server.accept_connections();

	return 0;
}
