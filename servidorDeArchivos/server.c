#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h> // Sockets
#include <netinet/in.h> //IPv4
#include <arpa/inet.h>
#include <errno.h>
#include "split.h"

#include "protocol.h"

#define MAX_CLIENTS 100

int atrClientsCount = 0;
static int atrUID = 10;
client_t * atrClients[MAX_CLIENTS];




/**
 * @brief Hilo request
 * @param prmArg identificador del cliente
 * @return NULL
 */
void *clientRequest(void *prmArg);
/**
 * @brief Imprimero la direccion IP del cliente
 * @param prmAddress 
 */
void print_client_addr(struct sockaddr_in prmAddress);
/**
 * @brief Agrega clientes a la lista-Cola
 * @param prmClient 
 */
void queue_add(client_t *prmClient);
/**
 * @brief Elimina clientes de la lista-Cola
 * @param prmUID 
 */
void queue_remove(int prmUID);
/**
 * @brief Funcion manejadora de SIGTERM
 * @param int De la senal recibida
 */
void handle_sigterm(int);



int atrFinishedServer;
int atrFinished;

int *atrID_clientsSockets; // Arreglo de "prmIndex" para los sockets de los clientes

/**
 * @brief main
 * @param argc = 2
 * @param argv[0] = Ejecutable
 * @param argv[1] = Puerto del servidor
 * @return int
 */
int main(int argc, char *argv[])
{
    int varPortServer;
    int varIndex;

    if (argc != 2)
    {
        fprintf(stderr, "Debe especificar el puerto del servidor.\n");
        exit(EXIT_FAILURE);
    }

    varPortServer = atoi(argv[1]);

    struct sigaction varAct;
    struct sigaction varOldAct;

    memset(&varAct, 0, sizeof(struct sigaction));
    memset(&varOldAct, 0, sizeof(struct sigaction));

    // Cuando se reciba SIGTERM se ejecutara handle_sigterm
    varAct.sa_handler = handle_sigterm;
    // Instalamos el navegador para SIGTERM
    sigaction(SIGTERM, &varAct, &varOldAct);

    int varOption = 1;
    // Socket del servidor
    int varServerSocket;
    // Socket del cliente
    int varClientSocket;
    // Direccion (IPv4)
    struct sockaddr_in varServerAddress;
    struct sockaddr_in varClientAddress;
    // Arreglo de ID de hilo para los clientes
    pthread_t *thread_client; 


    // 1. Socket IPv4, de tipo flujo (stream)
    varServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    // Preparar la direccion para asociarla al socket
    // Configuracion del socket
    memset(&varServerAddress, 0, sizeof(struct sockaddr_in));
    varServerAddress.sin_family = AF_INET;
    varServerAddress.sin_port = htons(varPortServer);
    varServerAddress.sin_addr.s_addr = INADDR_ANY; // 0.0.0.
    if (setsockopt(varServerSocket, SOL_SOCKET, (SO_REUSEPORT | SO_REUSEADDR), (char *)&varOption, sizeof(varOption)) < 0)
	{
		DieWithError("ERROR -1: setsockopt falló");
	}
    // 2. Asociar el socket a una direccion (IPv4)
    if (bind(varServerSocket, (struct sockaddr *)&varServerAddress, sizeof(struct sockaddr_in)) < 0)
    {
        DieWithError("Error -1: No se puede asocial es socket a la direccion IPv4");
    }
    // 3. Socket disponible
    if (listen(varServerSocket, 10) < 0)
    {
        DieWithError("Error -1: Socket no disponible");
    }
    
    printf("===      Servidor iniciado     ===\n");
	printf("=== Servidor en el puerto %d ===\n", varPortServer);

    atrFinishedServer = 0;
    varIndex = 0;

    while (!atrFinishedServer)
    {
        // Tamaño esperado de la direccion
        socklen_t varClientAddressLength;
        varClientAddressLength = sizeof(struct sockaddr_in); 
        // 4. Aceptar la conexion
        printf("Waiting response...\n");
        varClientSocket = accept(varServerSocket, (struct sockaddr *)&varClientAddress, &varClientAddressLength);

        if((atrClientsCount + 1) == MAX_CLIENTS)
        {
            printf("Se alcanzo el numero maximo de cliente");
            print_client_addr(varClientAddress);
            printf(":%d\n", varClientAddress.sin_port);
            close(varClientSocket);
            continue;
        }

        client_t * varClient = (client_t *) malloc(sizeof(client_t));
        varClient -> atrAddress = varClientAddress;
        varClient -> atrSocket = varClientSocket;
        varClient -> atrUID = atrUID++;

        // Crea un hilo pasandole como parametro varClientSocket (almacenado en un "arreglo")
        queue_add(varClient);
            // TODO: FALLA AL CREAR HILO
        pthread_create(&thread_client, NULL, &clientRequest, (void *)varClient);
        sleep(1);

        // Cerrar el socket original para liberar el puerto
        close(varServerSocket);
        exit(EXIT_SUCCESS);
    }
}

//TODO:
void *clientRequest(void *prmArg)
{
    //printf("%s Client connected\n", inet_ntoa(varServerAddress.sin_addr));
    int varIDSocket;
    varIDSocket = *(int *)prmArg; // Recupera el ID del socket del cliente
    while (!atrFinished)
    {
        request varRequest;
        memset(&varRequest, 0, sizeof(request));
        read(varIDSocket, (char *)&varRequest, sizeof(request));
        printf("Solicitud: operacion: %s archivo: %s\n",
               varRequest.atrOperation, varRequest.atrFileName);

        if (EQUALS(varRequest.atrOperation, "get"))
        {
            // TODO: Enviar arhivo
            // 1. Verificar si el archivo existe
            // 2. Enviar informacion del archivo al cliente (size = -1) si
            // el archivo no existe
            // 3. Si el arvhico existe:
            // 3.1 Abrir el archivo en modo lectura
            // 3.2 Leer una parte del archivo
            // 3.3 Escribir la parte al socket
            // 3.4 Repetir 3.2 miestras falte por leer
        }
        else if (EQUALS(varRequest.atrOperation, "put"))
        {
            // TODO: Recibir archivo
            // 2. Recibir la informacion del archivo del cliente
            // 3. Crear la ruta "Files/FILENAME"
            // 3.1 Abrir el archivo en modo escritura
            // 3.2 Leer una parte del socket
            // 3.3 Escribir la parte en el archivo
            // 3.4 Repetir 3.2 mientras falte por leer
        }
        else if (EQUALS(varRequest.atrOperation, "Exit"))
        {
            // Cerrar la conexion con el cliente (el hilo en cuestion cierra la conexion)
            printf("Closing connection with client...\n");
            close(varIDSocket);
            atrFinished = -1;
        }
    }
}
void print_client_addr(struct sockaddr_in prmAddress)
{
	printf("%d.%d.%d.%d",
		   prmAddress.sin_addr.s_addr & 0xff,
		   (prmAddress.sin_addr.s_addr & 0xff00) >> 8,
		   (prmAddress.sin_addr.s_addr & 0xff0000) >> 16,
		   (prmAddress.sin_addr.s_addr & 0xff000000) >> 24);

	// Imprime la direccion IP del cliente
}
void queue_add(client_t *prmClient)
{
	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		if (!atrClients[i])
		{
			atrClients[i] = prmClient;
			break;
		}
	}
}
void queue_remove(int prmUID)
{
	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		if (atrClients[i])
		{
			if (atrClients[i]->atrUID == prmUID)
			{
				atrClients[i] = NULL;
				break;
			}
		}
	}
}
void handle_sigterm(int atrSig)
{
    printf("SIGTERM Recibida! %d\n", atrSig);

    atrFinishedServer = 1;
    // Cerrar todos los recursos abiertos
    fclose(stdin);
}