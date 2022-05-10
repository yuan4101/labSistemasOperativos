#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

int esDirectorio (char[]);
int buscar(char[], char[]);

int main(int argc, char *argv[])
{
    printf("Busqueda de archivos\n\n");

    if (argc != 3){
        fprintf(stderr, "Ingrese un directorio y patron de busqueda!\n");
        exit(EXIT_FAILURE);
    }

    printf("Se encontraron '%d' resultados al buscar '%s' en la ruta '%s'\n", buscar(argv[1], argv[2]), argv[2], argv[1]);
    
    exit(EXIT_SUCCESS);
}

int esDirectorio(char *ruta){
    struct stat buf;

    if (stat(ruta, &buf) < 0){
        printf("Corrupted at: '%s'\n", ruta);
        perror("stat");
        return 0;
    }

    if (S_ISDIR(buf.st_mode)){
        return 1;
    }
    else{
        return 0;
    }
}

int buscar(char *directorio, char *patron){
    int total = 0;
    char * ruta = (char*) malloc(200 * sizeof(char));
    struct dirent *ent;
    DIR *dir = opendir(directorio);

    while ((ent = readdir(dir)) != NULL){
        strcpy(ruta, directorio);
        strcat(ruta, "/");
        strcat(ruta, ent->d_name);

        if (strstr(ent->d_name, patron) != NULL){
            printf("- %s\n", ruta);
            total = total + 1;
        }

        if (esDirectorio(ruta) && strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0){
            total = total + buscar(ruta, patron);
        }
    }

    free(ruta);

    closedir(dir);

    return total;
}