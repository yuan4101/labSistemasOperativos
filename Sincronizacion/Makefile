all: sem.o barber.o readwrite.o
	gcc -o barber barber.o sem.o -pthread
	gcc -o readwrite readwrite.o sem.o -pthread

sem.o: sem.c
	gcc -c -o sem.o sem.c

barber.o: barber.c
	gcc -c -o barber.o barber.c

readwrite.o: readwrite.c
	gcc -c -o readwrite.o readwrite.c

clean:
	rm -f *.o barber readwrite