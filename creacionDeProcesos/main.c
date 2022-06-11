#include <stdlib.h>
#include <stdio.h>
#include "split.h"

int main(int argc, char * argv[]) {
	split_list * list;
	int i;

	list = split(" hola mundo esta es una cadena     adios", " \t\n|");

	for (i= 0; i< list->count; i++) {
		printf("#%s# ", list->parts[i]);
	}
}