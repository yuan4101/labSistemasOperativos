all: server.o client.o split.o protocol.o
	gcc -o server server.o split.o -pthread
	gcc -o client client.o

server.o: server.c
	gcc -c -o server.o server.c

client.o: client.c
	gcc -c -o client.o client.c

protocol.o: protocol.c
	gcc -c -o protocol.o protocol.c
	
split.o: split.c
	gcc -c -o split.o split.c

clean:
	rm -rf *.o server client protocol files/