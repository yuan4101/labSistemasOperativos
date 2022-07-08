#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h> // Sockets
#include <netinet/in.h> //IPv4
#include <arpa/inet.h>

typedef struct
{
    char atrOperation[16];
    char atrFileName[PATH_MAX];
}request;

typedef struct
{
    int atrSize; /**< Tamano del archivo a tranferir, -1 si no existe >**/
    char atrFileNeme[PATH_MAX];
}file_info;

typedef struct
{
	struct sockaddr_in atrAddress;
	int atrSocket;
	int atrUID;
}client_t;

/**
 * @brief Lee los atributos de un archivo
 * @param prmFileName del archivo a leer
 * @return file_info 
 */
file_info read_file_atr(char * prmFileName);
/**
 * @brief Factoriza el uso de perror("errorMessage") y exit(EXIT_FAILURE)
 * @param errorMessage 
 */
void DieWithError(char *errorMessage);


#define EQUALS(s1, s2) (strcmp(s1, s2) == 0)

#endif