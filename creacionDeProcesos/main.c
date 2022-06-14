#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "split.h"

int main(int argc, char * argv[]) {
	pid_t varPid;
	int flagWaitStatus;
	int flagFinished;
	split_list * varList;

	char varLinea[BUFSIZ];
	/*
	split_list * list;
	int i;

	list = split(" hola mundo esta es una cadena     adios", " \t\n|");

	for (i= 0; i< list->count; i++) {
		printf("#%s# ", list->parts[i]);
	}
	*/

	flagFinished = 0;

	varList = split(varLinea, " \t\n|");

	while (!flagFinished)
	{
		memset(varLinea, 0, BUFSIZ)

		fgets(varLinea, BUFSIZ, stdin);
	}
	

	varPid = fork();

	if (varPid != 0)
	{
		waitpid(varPid, &flagWaitStatus, 0);
	} else {
		//int varResultado;
		execvp(varList-> parts[0], varlist->parts);
		perror("exec");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}