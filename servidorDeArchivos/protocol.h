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

file_info read_file_atr(char * prmFileName);

//int read_form_socket(file_info * varInfo, char * prmDestination)

#define EQUALS(s1, s2) (strcmp(s1, s2) == 0)

#endif