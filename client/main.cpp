// TCP Client

#include <iostream>

#include "message.h"
using namespace std;

int main(int argc, char **argv) {
	
	string text = "hello ## wo#rld";

	Message message = Message(text, true, true);
	cout << message.encode() << endl;
	
	return 0;
}
