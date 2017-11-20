all: chatserver

chatserver: main.cpp chatserver.o tcp_server.o ../tcp_connection.o ../message.o
	g++ -Wall -lpthread -std=gnu++11 -g main.cpp chatserver.o tcp_server.o tcp_connection.o message.o -o $@

%.o: %.cpp
	g++ -Wall -c -std=gnu++11 -g $^

clean:
	rm -rf chatserver *.o
