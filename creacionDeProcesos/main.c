#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "split.h"

int main(int argc, char * argv[]) {
	pid_t varPid;
	split_list * varList;
	int flagWaitStatus;
	char varLinea[BUFSIZ];

	int flagFinished = 0;

	while (!flagFinished)
	{
		memset(varLinea, 0, BUFSIZ);
		printf("$ ");
		fgets(varLinea, BUFSIZ, stdin);
		varList = split(varLinea, " \t\n|");

		if (varList->count == 0) {
			continue;
		}
		
		if (varList->parts[0][0] == '#') {
			continue;
		}

		if (strcmp(varList->parts[0], "exit") == 0
			|| strcmp(varList->parts[0], "logout") == 0
			|| strcmp(varList->parts[0], "quit") == 0) {
			flagFinished = 1;
			continue;
		}
		
		varPid = fork();

		if (varPid != 0) {
			waitpid(varPid, &flagWaitStatus, 0);
		} else {
			execvp(varList->parts[0], varList->parts);
			perror("exec");
			exit(EXIT_FAILURE);
		}
	}
	exit(EXIT_SUCCESS);
}