all: chatclient

chatclient: main.cpp client.o ../message.o ../tcp_connection.o
	g++ -Wall -lpthread -std=gnu++11 $^ -o $@

*.o: *.cpp
	g++ -Wall -std=gnu++11 -c $^

clean:
	rm -rf chatclient ../*.o *.o
