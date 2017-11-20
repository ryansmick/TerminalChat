all: chatserver

chatserver: main.cpp chatserver.o tcp_server.o ../tcp_connection.o ../message.o
	g++ -Wall -std=gnu++11 $^ -o $@

*.o: *.cpp
	g++ -Wall -c -std=gnu++11 $^

clean:
	rm -rf chatserver *.o ../*.o

