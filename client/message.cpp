// Message class implementation

#include "message.h"
#include <string>
#include <iostream>

Message::Message(string message_text, bool is_command, bool is_prompted) {

	this->message_text = message_text;
	this->is_command = is_command;
	this->is_prompted = is_prompted;

	this->encode_delimiter = '#';
	this->boolean_true_encoding = 'T';
	this->boolean_false_encoding = 'F';

}

string Message::encode() {

	string encoded_string = string(this->message_text);

	cout << encoded_string << endl;
	cout << is_command << endl;
	cout << is_prompted << endl;

	// Escape the delimiter
	for(size_t i = 0; i < encoded_string.length(); i++) {
		cout << encoded_string.at(i) << endl;
		if (encoded_string.at(i) == this->encode_delimiter) {
			encoded_string.insert(i, &(this->encode_delimiter));
			i++;
		}
	}

	encoded_string.append(&(this->encode_delimiter));

	cout << encoded_string << endl;

	// Add is_command
	if (this->is_command) {
		encoded_string.append(&(this->boolean_true_encoding));
	}
	else {
		encoded_string.append(&(this->boolean_false_encoding));
	}

	encoded_string.append(&(this->encode_delimiter));

	cout << encoded_string << endl;

	// Add is_prompted
	if (this->is_prompted) {
		encoded_string.append(&(this->boolean_true_encoding));
	}
	else {
		encoded_string.append(&(this->boolean_false_encoding));
	}

	cout << encoded_string << endl;

	return encoded_string;

}
