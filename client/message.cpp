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

	// Escape the delimiter
	for(size_t i = 0; i < encoded_string.length(); i++) {
		if (encoded_string.at(i) == this->encode_delimiter) {
			encoded_string.insert(i, 1, this->encode_delimiter);
			i++;
		}
	}

	encoded_string += this->encode_delimiter;


	// Add is_command
	if (this->is_command) {
		encoded_string += this->boolean_true_encoding;
	}
	else {
		encoded_string += this->boolean_false_encoding;
	}

	encoded_string += this->encode_delimiter;


	// Add is_prompted
	if (this->is_prompted) {
		encoded_string += this->boolean_true_encoding;
	}
	else {
		encoded_string += this->boolean_false_encoding;
	}

	return encoded_string;

}
