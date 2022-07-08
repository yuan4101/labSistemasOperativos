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
#include "split.h"
#include "protocol.h"

int atrFinished;

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
    int varPortServer = 0;
    varPortServer = atoi(argv[1]);

    // Socket del servidor
    int varServerSocket;

    // Socket del cliente
    int varClientSocket;

    // Direccion IPV4
    struct sockaddr_in varAddress;

    // 1. Socket IPv4, de tipo flujo (stream)
    varServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (varServerSocket == -1) {
        fprintf(stderr, "No se pudo crear el socket");
        exit(EXIT_FAILURE);
    }

    // Preparar la direccion para asociarla al socket
    memset(&varAddress, 0, sizeof(struct sockaddr_in));
    varAddress.sin_family = AF_INET;

    // Recibir el puerto a escuchar por la linea de comandos
    varAddress.sin_port = htons(varPortServer);

    // inet_aton("0.0.0.0", &varAddress.sin_addr);
    varAddress.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0

    // 2. Asociar el socket a una direccion (IPv4)
    if (bind(varServerSocket, (struct sockaddr *)&varAddress, sizeof(struct sockaddr_in)) == -1) {
        fprintf(stderr, "No se puede asociar el socket a la direccion IPv4");
        exit(EXIT_FAILURE);
    }

    // 3. Socket disponible
    if (listen(varServerSocket, 10) == -1) {
        fprintf(stderr, "Socket no disponible");
        exit(EXIT_FAILURE);
    }

    socklen_t varAddressLength;
    varAddressLength = sizeof(struct sockaddr_in); // Tamaño esperado de la direccion

    // 4. Aceptar la conexion
    printf("Waiting connection...\n");
    varClientSocket = accept(varServerSocket, (struct sockaddr *)&varAddress, &varAddressLength);
    if (varClientSocket == -1) {
        fprintf(stderr, "No se pudo obtener la descripcion del socket");
        exit(EXIT_FAILURE);
    }
    printf("%s Client connected\n", inet_ntoa(varAddress.sin_addr));
    write(varClientSocket, "Welcome!\n", 10);

    atrFinished = 0;

    while (!atrFinished)
    {
        // Recibir mensaje
        int varNumBytes;
        request varRequest;
        memset(&varRequest, 0, sizeof(request));
        varNumBytes = read(varClientSocket, (char *)&varRequest, sizeof(request));
        if (varNumBytes == 0) {
            fprintf(stderr, "Client disconnected unexpectedly or EOF (read)\n");
            exit(EXIT_FAILURE);
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
        else if (EQUALS(varRequest.atrOperation, "exit")) {
            // Cerrar la conexion con el cliente (el hilo en cuestion cierra la conexion)
            printf("Closing connection with client...\n");
            close(varClientSocket);
            atrFinished = 1;
        }
    }

    printf("Closing server...\n");

    // Cerrar el socket original para liberar el puerto
    close(varServerSocket);

    exit(EXIT_SUCCESS);
}

void handler_sigterm(int atrSig)
{
    printf("SIGTERM Recibida! %d\n", atrSig);
    atrFinished = 1;
    // Cerrar todos los recursos abiertos
    fclose(stdin);
}