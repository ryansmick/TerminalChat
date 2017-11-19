all: TerminalChatClient

TerminalChatClient: main.cpp ../message.o ../tcp_connection.o
	g++ -Wall -std=gnu++11 $^ -o $@

*.o: *.cpp
	g++ -Wall -std=gnu++11 -c $^

clean:
	rm -rf TerminalChatClient *.o
