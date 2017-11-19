// Message class Definition

#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

using namespace std;

// Class to represent a message sent over the TCP connection
class Message {

	private:
		
		string message_text; // The body of the message
		bool is_command; // Is the message a command or data
		bool is_prompted; // Whether or not the message was received in response to a previous message
		char encode_delimiter;
		char boolean_true_encoding;
		char boolean_false_encoding;

	public:

		Message(string message_text, bool is_command, bool is_prompted);

		// Encode and decode messages
		string encode();
		static Message* decode(string encoded_message);

		// Getters and setters
		string get_message_text();
		bool get_is_command();
		bool get_is_prompted();
		void set_message_text(string new_message_text);
		void set_is_command(bool is_command);
		void set_is_prompted(bool is_prompted);

};

#endif
