CC=gcc

all: main.0 split.o
	$(CC) -o myshell main.o split.o

main.o: main.c
	$(CC) -c -0 main.o main.c

split.o: split.c
	$(CC) -c -0 split.o split.c

clean:
	rm -f myshell main.o split.0