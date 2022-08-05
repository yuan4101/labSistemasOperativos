all: main.o kbuf.o
	gcc -o gestionMemoriaDinamica main.o kbuf.o

main.o: main.c
	gcc -c -o main.o main.c

kbuf.o: kbuf.c
	gcc -c -o kbuf.o kbuf.c

clean:
	rm -f gestionMemoriaDinamica main.o kbuf.o
	clear