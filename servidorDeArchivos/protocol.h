#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <sys/socket.h> // Sockets
#include <netinet/in.h> //IPv4
#include <arpa/inet.h>

#include <pthread.h>
#include <limits.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>

#define EQUALS(varString1, varString2) (strcmp(varString1, varString2) == 0)
#define NOEQUALS(varString1, varString2) (strcmp(varString1, varString2) != 0)

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

#endif