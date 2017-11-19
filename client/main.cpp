// TCP Client

#include <iostream>

#include "message.h"
using namespace std;

int main(int argc, char **argv) {
	
	string text = "hello # world";

	Message message = Message(text, false, false);
	cout << message.encode() << endl;
	
	return 0;
}
