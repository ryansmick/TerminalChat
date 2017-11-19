// TCP Client

#include <iostream>
#include "message.h"

using namespace std;

int main(int argc, char **argv) {
	
	string text = "hello ## wo#rld###";
	cout << text << endl;

	Message message = Message(text, true, false);
	Message new_message = Message::decode(message.encode());
	cout << new_message.get_message_text() << endl;
	cout << new_message.get_is_command() << endl;
	cout << new_message.get_is_prompted() << endl;
	
	return 0;
}
