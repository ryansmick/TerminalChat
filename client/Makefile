all: chatclient

chatclient: main.cpp client.o ../message.o ../tcp_connection.o
	g++ -Wall -lpthread -std=gnu++11 -g  main.cpp client.o message.o tcp_connection.o -o $@

%.o: %.cpp
	g++ -Wall -std=gnu++11 -g -c $^

clean:
	rm -rf chatclient *.o
