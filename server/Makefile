all: chatserver

chatserver: main.cpp tcp_server.o ../tcp_connection.o ../message.o
	g++ -Wall -std=gnu++11 main.cpp tcp_server.o tcp_connection.o message.o -o $@

*.o: *.cpp
	g++ -Wall -c -std=gnu++11 $^

clean:
	rm -rf chatserver *.o
