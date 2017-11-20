// TCP server

#include <iostream>

#include "../tcp_connection.h"
#include "tcp_server.h"

using namespace std;

int main(int argc, char **argv) {
	
	TCPServer server = TCPServer();
	server.start_server(argv[1]);
	
	TCPConnection conn = server.accept_connection();

	while(true) {
		if(conn.is_message_available()) {
			Message m = conn.get_latest_message();
			cout << m.get_message_text() << endl;
			conn.pop_latest_message();
		}
	}

	return 0;
}
