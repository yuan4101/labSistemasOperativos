#include <sys/socket.h> // Sockets
#include <netinet/in.h> //IPv4
#include <arpa/inet.h>

#include <pthread.h>
#include <semaphore.h>
#include <signal.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

#include "split.h"
#include "protocol.h"

#define MAX_CLIENTS 100

int atrClientsCount = 0;
static int atrUID = 10;
client_t *atrClients[MAX_CLIENTS];
int atrFinishedServer = 0;

/**
 * @brief Tipo de dato del cliente
 */
typedef struct
{
    struct sockaddr_in atrAddress;
    int atrSocket;
    int atrUID;
} client_t;

/**
 * @brief Hilo request
 * @param prmArg identificador del cliente
 * @return NULL
 */
void *client_request(void *prmArg);
/**
 * @brief Envia un mensaje al cliente
 * @param prmMessage 
 * @param prmClient 
 */
void write_message_client(char *prmMessage, client_t *prmClient);
/**
 * @brief Imprimero la direccion IP del cliente
 * @param prmAddress
 */
void print_client_addr(struct sockaddr_in prmAddress);
/**
 * @brief Agrega clientes a la lista-Cola
 * @param prmClient
 */
void add_client_queue(client_t *prmClient);
/**
 * @brief Elimina clientes de la lista-Cola
 * @param prmUID
 */
void remove_client_queue(int prmUID);
/**
 * @brief Funcion que maneja el SIGTERM
 * @param int De la señal recibida
 */
void handler_sigterm(int);

/**
 * @brief main
 * @param argc = 2
 * @param argv[0] = Ejecutable
 * @param argv[1] = Puerto del servidor
 * @return int
 */
int main(int argc, char *argv[])
{

    //#####################   SIGTERM   ########################
    struct sigaction varAct;
    struct sigaction varOldAct;
    memset(&varAct, 0, sizeof(struct sigaction));
    memset(&varOldAct, 0, sizeof(struct sigaction));


    // Cuando se reciba SIGTERM se ejecutara handler_sigterm
    varAct.sa_handler = handler_sigterm;

    // Instalamos el navegador para SIGTERM
    sigaction(SIGTERM, &varAct, &varOldAct);

    //###########################################################

    //Lectura de puerto desde consola
    if (argc != 2) {
        fprintf(stderr, "Debe especificar el puerto del servidor.\n");
        exit(EXIT_FAILURE);
    }
    int varPortServer = atoi(argv[1]);
    int varOption = 1;

    // Socket del servidor
    int varServerSocket;

    // Socket del cliente
    int varClientSocket;

    // Direccion IPV4
    struct sockaddr_in varServerAddress;
    struct sockaddr_in varClientAddress;
    
    // Arreglo de ID de hilo para los clientes
    pthread_t thread_client;

    // 1. Socket IPv4, de tipo flujo (stream)
    varServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (varServerSocket == -1) {
        fprintf(stderr, "No se pudo crear el socket");
        exit(EXIT_FAILURE);
    }

    // Preparar la direccion para asociarla al socket
    memset(&varServerAddress, 0, sizeof(struct sockaddr_in));
    varServerAddress.sin_family = AF_INET;

    // Recibir el puerto a escuchar por la linea de comandos
    varServerAddress.sin_port = htons(varPortServer);

    // inet_aton("0.0.0.0", &varAddress.sin_addr);
    varServerAddress.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    
    if (setsockopt(varServerSocket, SOL_SOCKET, (SO_REUSEPORT | SO_REUSEADDR), (char *)&varOption, sizeof(varOption)) < 0)
    {
        DieWithError("ERROR -1: setsockopt falló");
    }

    // 2. Asociar el socket a una direccion (IPv4)
    if (bind(varServerSocket, (struct sockaddr *)&varServerAddress, sizeof(struct sockaddr_in)) < 0)
    {
        DieWithError("Error -1: No se puede asociar el socket a la direccion IPv4");
    }

    // 3. Socket disponible
    if (listen(varServerSocket, 10) == -1) {
        DieWithError("Error -1: Socket no disponible");
    }

    printf("===      Servidor iniciado     ===\n");
    printf("=== Servidor en el puerto %d ===\n", varPortServer);


    while (!atrFinishedServer)
    {
        // Tamaño esperado de la direccion
        socklen_t varClientAddressLength;
        varClientAddressLength = sizeof(struct sockaddr_in);
        // 4. Aceptar la conexion
        printf("Waiting connection...\n");
        varClientSocket = accept(varServerSocket, (struct sockaddr *)&varClientAddress, &varClientAddressLength);

        if ((atrClientsCount + 1) == MAX_CLIENTS)
        {
            printf("Se alcanzo el numero maximo de cliente");
            print_client_addr(varClientAddress);
            printf(":%d\n", varClientAddress.sin_port);
            close(varClientSocket);
            continue;
        }

        client_t *varClient = (client_t *)malloc(sizeof(client_t));
        varClient->atrAddress = varClientAddress;
        varClient->atrSocket = varClientSocket;
        varClient->atrUID = atrUID++;

        // Crea un hilo pasandole como parametro varClientSocket (almacenado en un "arreglo")
        add_client_queue(varClient);
        pthread_create(&thread_client, NULL, &client_request, (void *)varClient);

        sleep(1);
    }
    // Cerrar el socket original para liberar el puerto
    close(varServerSocket);
    return EXIT_SUCCESS;
}


void *client_request(void *prmArg)
{
    int varFinished = 0;

    client_t *varClient = (client_t *)prmArg; // Recupera el cliente pasado por parametro

    char varBufferOut[BUFSIZ];
    memset(varBufferOut, 0, BUFSIZ);

    atrClientsCount++;
    sprintf(varBufferOut, "%s Client connected\nClients connected : %d", inet_ntoa(varClient->atrAddress.sin_addr), atrClientsCount);
    printf("%s", varBufferOut);
    write_message_client(varBufferOut, varClient);

    while (!varFinished)
    {
        // Recibir mensaje
        int varNumBytes;
        request varRequest;
        memset(&varRequest, 0, sizeof(request));
        
        varNumBytes = read(varClient->atrSocket, (char *)&varRequest, sizeof(request));
        if (varNumBytes == 0) {
            fprintf(stderr, "Client disconnected unexpectedly or EOF (read)\n");
            varFinished = -1;
        }
        else if (varNumBytes == -1) {
            fprintf(stderr, "Error (read)\n");
            continue;
        }

        printf("Operation: %s\nFilename: %s\n", varRequest.atrOperation, varRequest.atrFileName);

        if (EQUALS(varRequest.atrOperation, "get")) {
            printf("OPERATION GET...\n");
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
        else if (EQUALS(varRequest.atrOperation, "put")) {
            printf("OPERATION PUT...\n");
            // TODO: Recibir archivo
            // 2. Recibir la informacion del archivo del cliente
            // 3. Crear la ruta "Files/FILENAME"
            // 3.1 Abrir el archivo en modo escritura
            // 3.2 Leer una parte del socket
            // 3.3 Escribir la parte en el archivo
            // 3.4 Repetir 3.2 mientras falte por leer
        }
        else if (EQUALS(varRequest.atrOperation, "exit"))
        {
            varFinished = -1;
        }
        else
        {
            printf("La operacion recibida no existe...\n");
        }
    }
    
    // Cerrar la conexion con el cliente (el hilo en cuestion cierra la conexion)
    atrClientsCount--;
    memset(&varBufferOut, 0, BUFSIZ);
    sprintf(varBufferOut, "Closing connection with client %s...\nClients connected : %d", inet_ntoa(varClient->atrAddress.sin_addr), atrClientsCount);
    printf("%s", varBufferOut);
    write_message_client(varBufferOut, varClient);
    
    // Elimina el cliente del socket
    close(varClient->atrSocket);
    
    // Elimina el cliente de la cola
    remove_client_queue(varClient->atrUID);
    free(varClient);
    
    // Libera el hilo de la ejecucion
    pthread_detach(pthread_self());

    return NULL;
}
void write_message_client(char *prmMessage, client_t *prmClient)
{
    if (write(prmClient->atrSocket, prmMessage, strlen(prmMessage)) == -1)
    {
        DieWithError("ERROR -1: error al enviar el mensaje");
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
void add_client_queue(client_t *prmClient)
{
    for (int varIndex = 0; varIndex < MAX_CLIENTS; ++varIndex)
    {
        if (!atrClients[varIndex])
        {
            atrClients[varIndex] = prmClient;
            break;
        }
    }
}
void remove_client_queue(int prmUID)
{
    for (int varIndex = 0; varIndex < MAX_CLIENTS; ++varIndex)
    {
        if (atrClients[varIndex])
        {
            if (atrClients[varIndex]->atrUID == prmUID)
            {
                atrClients[varIndex] = NULL;
                break;
            }
        }
    }
}
void handler_sigterm(int atrSig)
{
    printf("SIGTERM Recibida! %d\n", atrSig);
    atrFinished = 1;
    // Cerrar todos los recursos abiertos
    fclose(stdin);
}