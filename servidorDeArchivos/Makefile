all: server.o client.o protocol.o
	gcc -o server server.o -pthread
	gcc -o client client.o

server.o: server.c
	gcc -c -o server.o server.c

client.o: client.c
	gcc -c -o client.o client.c

protocol.o: protocol.c
	gcc -c -o protocol.o protocol.c

clean:
	rm -rf *.o server client protocol files/