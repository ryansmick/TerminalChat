all: TerminalChatServer

TerminalChatServer: main.cpp tcp_server.o ../tcp_connection.o ../message.o
	g++ -Wall -std=gnu++11 $^ -o $@

*.o: *.cpp
	g++ -Wall -std=gnu++11 -c $^

clean:
	rm -rf TerminalChatServer *.o ../*.o
