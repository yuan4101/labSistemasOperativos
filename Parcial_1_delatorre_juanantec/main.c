#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define VERSIONS_DB "versions.db"
#define VERSIONS_DIR ".versions"
#define VERSIONS_PATH VERSIONS_DIR "/" VERSIONS_DB

/**
 * @brief 
 * Crear el directorio de versiones 
 * @return 1 si el directorio existe y/o se puede crear
 */
int crear_directorio_versiones();

int main(int argc, char * argv[])
{
    //Crear directorio
    if(crear_directorio_versiones() == 0)
    {
        fprintf(stderr, "No se puede crear el directorio %s\n", VERSIONS_DIR);
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

int crear_directorio_versiones()
{
    struct stat varStat;

    //Verifica si el directorio existe
    if(stat(VERSIONS_DIR, &varStat) == 0)
    {
        if (S_ISDIR(varStat.st_mode))
            return 1;
        else
            return 0;
    }

    //Verifica si se puede crear el directorio
    if(stat(VERSIONS_DIR, &varStat) != 0)
    {
        if (mkdir(VERSIONS_DIR, 0755) != 0)
        {
            fprintf(stderr, "No se puede crear el directorio de versiones");
            return 0;
        }    
        else
            return 1;
    }
}