// Message class implementation

#include "message.h"
#include <string>
#include <iostream>

Message::Message(string message_text, bool is_command, bool is_prompted) {

	this->message_text = message_text;
	this->is_command = is_command;
	this->is_prompted = is_prompted;

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

Message& Message::decode(string encoded_message) {

	string text = encoded_message.substr(0, encoded_message.length() - 4);
	bool is_command = encoded_message.at(encoded_message.length() - 3) == boolean_true_encoding ? true : false;
	bool is_prompted = encoded_message.at(encoded_message.length() - 1) == boolean_true_encoding ? true : false;

	// Remove the extra delimiters from the string
	for(size_t i = 0; i < text.length()-1; i++) {
		if(text.at(i) == encode_delimiter && text.at(i+1) == encode_delimiter) {
			text.erase(i+1, 1);
		}
	}

	Message* new_message = new Message(text, is_command, is_prompted);

	return *new_message;

}

string Message::get_message_text() {
	return this->message_text;
}

bool Message::get_is_command() {
	return this->is_command;
}

bool Message:: get_is_prompted() {
	return this->is_prompted;
}

void Message::set_message_text(string new_message_text) {
	this->message_text = new_message_text;
}

void Message::set_is_command(bool is_command) {
	this->is_command = is_command;
}

void Message::set_is_prompted(bool is_prompted) {
	this->is_prompted = is_prompted;
}
