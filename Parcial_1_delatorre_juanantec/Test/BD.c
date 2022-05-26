#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

//hexdump -C --Herramienta para abrir la BD

typedef struct{
    char varFilename[PATH_MAX];
    char varComment[BUFSIZ];
    char varHash[65];
}file_version;
/*
*@brieft Fuarda una nueva version en la base de datos
*@param v Version a guardar
*@return 1 en caso de exito, 0 si  ocurre un error
*/
int save(file_version *v);
/*
*@brieft Fuarda una nueva version en la base de datos
*@param v Version a guardar
*@return 1 en caso de exito, 0 si  ocurre un error
*/
int save2(file_version *v);

#define VERSIONS_FILE "test_versions.dv"

int main (int argc, char * argv[]){

    file_version v;

    memset(&v, 0, sizeof(file_version)); //Limpiar espacion de memoria en la pila
    strcpy(v.varFilename, "hello.c");
    strcpy(v.varComment, "Primera version");
    strcpy(v.varHash, "jwei23293r4nfu49d9dnx0x0c00v00c042349234283cc3929492939423c33423");

    if (!save(&v))
    {
        fprintf(stderr, "No se pudo guardar \n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

int save(file_version *v){
    //Abrir la base de datos
    int fd;
    ssize_t nbytes;
    
    fd = open(VERSIONS_FILE, //ARHCIVO
    O_CREAT | O_WRONLY | O_APPEND, //FLAGS
    S_IRWXG | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH //MODO
    );

    if(fd < 0) 
    {
        perror("open");
        return 0;
    }

    nbytes = write(
        fd, // Descriptor de archivo
        v, //Referencia a memoria (Buffer)
        sizeof(file_version));

    close(fd);

    if (nbytes ==  sizeof(file_version))
        return 1;
    else
        return 0;
    

    //Cerrar la base de datos
    close(fd);

    return 0;
}

int save2(file_version *v){
    //Abrir la base de datos
    FILE *fp;
    int nmemb;
    
    fp = open(VERSIONS_FILE, "a");

    if(fp < NULL) 
    {
        perror("fopen");
        return 0;
    }

    nmemb = fwrite(
        v, //Referencia a memoria (Buffer)
        sizeof(file_version), //Tamano de la memoria de lo que se va a escribir
        1,
        fp
        );

    fclose(fp);

    if (nmemb == 1)
        return 1;
    else
        return 0;
    

    //Cerrar la base de datos
    close(fp);

    return 0;
}