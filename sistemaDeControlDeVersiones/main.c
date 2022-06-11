#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/stat.h>

#define VERSIONS_DB "versions.db"
#define VERSIONS_DIR ".versions"
#define VERSIONS_PATH VERSIONS_DIR "/" VERSIONS_DB

typedef struct strRegistroArchivo{
	char atrNombreArchivo[PATH_MAX];
	char atrComentario[BUFSIZ];
	char atrHash[65];
}strRegistroArchivo;

void list(char *);
int buscar(char *);
void get(char *, int);
int add(char *, char *);
char * obtenerHash(char *);
int crearDirectorioVersiones();
int copiarArchivo(char *, char *);
void adicionarVersion(strRegistroArchivo);

int main(int argc, char * argv[])
{
    int varNumArgumentos = argc;

    if (varNumArgumentos == 1)
        if (crearDirectorioVersiones() == 0)
            exit(EXIT_FAILURE);
        else
			printf("%s Created successfully\n", VERSIONS_DIR);
			
    if (varNumArgumentos == 3)
		if (strcmp(argv[1], "list") == 0)
			list(argv[2]);

    if (varNumArgumentos == 4)
		if (strcmp(argv[1], "add") == 0)
			if (add(argv[2], argv[3]) == 1)
				printf("El archivo ha sido agregado\n");
			else
				printf("No se pudo añadir el archivo\n");
		else if( strcmp(argv[1], "get") == 0)
			get(argv[3], (atoi)(argv[2]));

    exit(EXIT_SUCCESS);
}

/**
 * @brief 
 * Crear el directorio de versiones 
 * @return 1 si el directorio existe y/o se puede crear 0 en caso contrario
 */
int crearDirectorioVersiones()
{
    struct stat varStat;

    if(stat(VERSIONS_DIR, &varStat) == 0)
        if (S_ISDIR(varStat.st_mode))
            return 1;
        else
		{
            fprintf(stderr, "%s no es un directorio\n", VERSIONS_DIR);
            return 0;
        }

    if(stat(VERSIONS_DIR, &varStat) != 0)
        if (mkdir(VERSIONS_DIR, 0755) != 0)
        {
            fprintf(stderr, "No se puede crear el directorio de versiones\n");
            return 0;
        }    
        else
		{
			FILE * varDBFilePointer;
			varDBFilePointer = fopen(".versions/versions.db", "a");
			fclose(varDBFilePointer);
			return 1;
		}
            
}

/**
 * @brief
 * Calcula el hash de un archivo con sha256
 * @param prmNombreArchivo  Nombre del archivo de entrada
 * @return Hash del archivo, NULL si ocurre un error
 */
char * obtenerHash(char * prmNombreArchivo)
{
	char * varResultado = NULL;
    char * varComando;
    FILE * varFilePointer;

    varComando = (char*) malloc(strlen("sha256sum ") + strlen(prmNombreArchivo) + 1);

    sprintf(varComando, "sha256sum %s", prmNombreArchivo);

    varFilePointer = popen(varComando, "r");

    if (varFilePointer == NULL)
	{
        free(varComando);
        return NULL;
    }

    varResultado = (char*) malloc(65);

    fscanf(varFilePointer,"%s",varResultado);

    if (strlen(varResultado) == 64)
	{
        pclose(varFilePointer);
        free(varComando);
        return varResultado;
    }
	else
	{
        pclose(varFilePointer);
        free(varComando);
        free(varResultado);
        return NULL;
    }

    return varResultado;
}

/**
 * @brief
 * Abre el archivo original y copia su contenido al nuevo archivo.
 * @param prmFuente Archivo fuente
 * @param prmDestino Archivo destino
 * @return 1 si no ocurrio ningun error y 0 si ocurrio un error
 */
int copiarArchivo(char * prmFuente, char * prmDestino)
{
	char varChar;
	FILE * varFilePointer, * varDestinyFilePointer;
	
	varFilePointer = fopen(prmFuente, "r");

	if (varFilePointer == NULL)
	{
		fprintf(stderr, "No se pudo abrir el archivo fuente %s \n", prmFuente);
		return 0;
	}

	varDestinyFilePointer = fopen(prmDestino, "w+");

	if (varDestinyFilePointer == NULL)
	{
		fprintf(stderr, "No se pudo abrir el archivo destino %s \n", prmDestino);
		return 0;
	}
	
	varChar = fgetc(varFilePointer);

	while( varChar != EOF )
	{
		fputc(varChar, varDestinyFilePointer);
        varChar = fgetc(varFilePointer);
	}
	
	fclose(varFilePointer);
	fclose(varDestinyFilePointer);

	return 1;
}

/**
 * @brief 
 * Agrega un nuevo registro al archivo "versions.db".
 * @param prmRegistroArchivo La estructura del registro
 */
void adicionarVersion(strRegistroArchivo prmRegistroArchivo)
{
	FILE * varDBFilePointer;
	varDBFilePointer = fopen(".versions/versions.db", "a");

	if (varDBFilePointer == NULL)
	{
		fprintf(stderr, "Error al abrir el archivo versions.db\n");
		exit(0);
	}

    fwrite(&prmRegistroArchivo, sizeof(struct strRegistroArchivo), 1, varDBFilePointer);
    fclose(varDBFilePointer);
}

/**
 * @brief 
 * Agrega un archivo al repositorio con un comentario
 * @param prmArchivo Archivo a agregar
 * @param prmComentario Cadena de caracteres asociada al archivo
 * @return Retorna 1 si el archivo fue agregado correctamente, 0 en caso contrario
 */
int add(char * prmArchivo, char * prmComentario)
{
	int varFlag = 0;
	struct stat varStat;
	char * varHash = malloc(sizeof(char)*64);
	char * varNuevo = malloc(sizeof(char)*100);
	struct strRegistroArchivo varRegistroArchivo;

	if (stat(prmArchivo, &varStat) < 0 )
	{
		fprintf(stderr, "El archivo no existe\n");
		return 0;
	}

	if (!S_ISREG(varStat.st_mode))
	{
		fprintf(stderr, "No es un archivo normal\n");
		return 0;
	}

	varHash = obtenerHash(prmArchivo);

	if (buscar(varHash) == 1)
		return 0;
	
	strcat(strcpy(varNuevo, ".versions/"), varHash);

	if (copiarArchivo(prmArchivo, varNuevo) == 1)
	{
		memset(&varRegistroArchivo, 0, sizeof(strRegistroArchivo));
		strcpy(varRegistroArchivo.atrNombreArchivo, prmArchivo);
		strcpy(varRegistroArchivo.atrComentario, prmComentario);
		strcpy(varRegistroArchivo.atrHash, varHash);
		adicionarVersion(varRegistroArchivo);
		varFlag = 1;
	}

	free(varHash);
	free(varNuevo);
	
	return varFlag;
}

/**
 * @brief 
 * Obtiene una versión solicitada (numero secuencial) de un archivo
 * @param prmNombreArchivo Nombre del archivo a obtener
 * @param prmVersion Numero de la version a obtener
 */
void get(char * prmNombreArchivo, int prmVersion)
{
	int varCount = 1;
	FILE * varFilePointer;
	struct strRegistroArchivo varRegistroArchivo;
	char * varFileVersion = malloc(sizeof(char) * 100);
	
	varFilePointer = fopen(".versions/versions.db", "r");

	if( varFilePointer == NULL)
	{
		fprintf(stderr, "No se pudo abrir el archivo versions.db\n");
		exit(0);
	}

	while (!feof(varFilePointer))
	{
		if (fread(&varRegistroArchivo, sizeof(strRegistroArchivo), 1, varFilePointer) != 1)
			break;

		if (strcmp(varRegistroArchivo.atrNombreArchivo, prmNombreArchivo) == 0)
			if(varCount == prmVersion)
			{
				strcat(strcpy(varFileVersion, ".versions/"), varRegistroArchivo.atrHash);
				copiarArchivo(varFileVersion, varRegistroArchivo.atrNombreArchivo);
				break;
			}
			else
				varCount = varCount + 1;
	}

	fclose(varFilePointer);
}

/**
 * @brief 
 * Imprime la información que se encuentra dentro del archivo versions.db del sub-directorio VERSIONS_DIR
 * @param prmNombreArchivo Nombre del archivo a listar
 */
void list(char * prmNombreArchivo)
{
	int varCount = 1;
	FILE * varFilePointer;
	struct strRegistroArchivo varRegistroArchivo;

	varFilePointer = fopen(".versions/versions.db", "r");

	if (varFilePointer == NULL)
	{
		fprintf(stderr, "No hay archivos con ese nombre\n");
		exit(0);
	}

	while (!feof(varFilePointer))
	{
		if (fread(&varRegistroArchivo, sizeof(strRegistroArchivo), 1, varFilePointer) != 1)
			break;
		
		if (strcmp(varRegistroArchivo.atrNombreArchivo, prmNombreArchivo) == 0 )
		{
			printf("Version: %d\nComentario: %s\nHash: %s\n\n", varCount, varRegistroArchivo.atrComentario, varRegistroArchivo.atrHash);
			varCount = varCount + 1;
		}
	}

	fclose(varFilePointer);
}

/**
 * @brief 
 * Busca un hash en VERSIONS_DB
 * @param prmHash El hash a buscar
 * @return Retorna 1 si encuentra coincidencia, 0 si no encuentra coincidencia
 */
int buscar(char * prmHash)
{
	FILE * varFilePointer;
	struct strRegistroArchivo varRegistroArchivo;
	
	varFilePointer = fopen(".versions/versions.db", "r");

	if( varFilePointer == NULL)
	{
		fprintf(stderr, "No se pudo abrir el archivo versions.db\n");
		exit(0);
	}

	while (!feof(varFilePointer))
	{
		if (fread(&varRegistroArchivo, sizeof(strRegistroArchivo), 1, varFilePointer) != 1)
			break;

		if (strcmp(varRegistroArchivo.atrHash, prmHash) == 0)
		{
			fclose(varFilePointer);
			return 1;
		}
	}

	fclose(varFilePointer);

	return 0;
}