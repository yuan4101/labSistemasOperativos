all: main.o split.o
	gcc -o myshell main.o split.o

main.o: main.c
	gcc -c -o main.o main.c

split.o: split.c
	gcc -c -o split.o split.c

clean:
	rm -f myshell main.o split.o