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

#include "protocol.h"

int atrFinished;

/**
 * @brief Funcio manejadora de SIGTERM
 * @param int De la senal resivida
 */
void handle_sigterm(int);

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

    if (argc != 2)
    {
        fprintf(stderr, "Debe especificar el puerto del servidor.\n");
        exit(EXIT_FAILURE);
    }

    varPortServer = 0;
    varPortServer = atoi(argv[1]);

    struct sigaction varAct;
    struct sigaction varOldAct;

    memset(&varAct, 0, sizeof(struct sigaction));
    memset(&varOldAct, 0, sizeof(struct sigaction));
    
    //Cuando se reciva SIGTERM se ejecutara handle_sigterm
    varAct.sa_handler = handle_sigterm;
    //Instalamos el navegador para SIGTERM
    sigaction(SIGTERM, &varAct, &varOldAct);

    // Socket del servidor
    int varServerSocket;
    // Socket del cliente
    int varClientSocket;
    // Direccion del servidor (IPv4)
    struct sockaddr_in varAddress;

    // 1. Socket IPv4, de tipo flujo (stream)
    varServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (varServerSocket == -1)
    {
        perror("Error -1: No se pudo crear el socket");
        exit(EXIT_FAILURE);
    }
    // Preparar la direccion para asociarla al socket
    memset(&varAddress, 0, sizeof(struct sockaddr_in));
    varAddress.sin_family = AF_INET;
        // Testing: Recibir el puerto a escuchar por la linea de comandos
    varAddress.sin_port = htons(varPortServer);
    // inet_aton("0.0.0.0", &varAddress.sin_addr);
    varAddress.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0

    // 2. Asociar el socket a una direccion (IPv4)
    if (bind(varServerSocket, (struct sockaddr *)&varAddress, sizeof(struct sockaddr_in)) == -1)
    {
        perror("Error -1: No se puede asocial es socket a la direccion IPv4");
        exit(EXIT_FAILURE);
    }
    // 3. Socket disponible
    if (listen(varServerSocket, 10) == -1)
    {
        perror("Error -1: Socket no disponible");
        exit(EXIT_FAILURE);
    }

    // el sistema "llena" varClientAddress con la informacion del cliente
    // y almacena en varClientAddressLength el tamaño obtenido de esa direccion
    struct sockaddr_in varClientAddress;
    memset(&varClientAddress, 0, sizeof(struct sockaddr_in));
    socklen_t varClientAddressLength;
    varClientAddressLength = sizeof(struct sockaddr_in); // Tamaño esperado de la direccion

    atrFinished = 0;
    while (!atrFinished)
    {
        // 4. Aceptar la conexion
        printf("Waiting for conections...\n");
        varClientSocket = accept(varServerSocket, (struct sockaddr *)&varClientAddress, &varClientAddressLength);
        if(varClientSocket == -1){
            perror("Error -1: No se pudo obtener la descripcion del socket");
            exit(EXIT_FAILURE);
        }
        printf("Client coneccted!\n");

        // TODO Comunicacion
        // Crear un hilo pasandole como parametro varClientSocket (almacenado en un "arreglo")
        sleep(5);

        request varRequest;
        memset(&varRequest, 0, sizeof(request));
        read(varClientSocket, (char*)&varRequest, sizeof(request));
        printf("Solicitud: operacion: %s archivo: %s\n", 
                varRequest.atrOperation, varRequest.atrFileName);

        if(EQUALS(varRequest.atrOperation, "get")){
            //TODO: Enviar arhivo
            //1. Verificar si el archivo existe
            //2. Enviar informacion del archivo al cliente (size = -1) si
            //el archivo no existe
            //3. Si el arvhico existe:
            //3.1 Abrir el archivo en modo lectura
            //3.2 Leer una parte del archivo
            //3.3 Escribir la parte al socket
            //3.4 Repetir 3.2 miestras falte por leer
        }else if(EQUALS(varRequest.atrOperation, "put")){
            //TODO: Recibir archivo
            //2. Recibir la informacion del archivo del cliente
            //3. Crear la ruta "Files/FILENAME"
            //3.1 Abrir el archivo en modo escritura
            //3.2 Leer una parte del socket
            //3.3 Escribir la parte en el archivo
            //3.4 Repetir 3.2 mientras falte por leer
        }else if(EQUALS(varRequest.atrOperation, "Exit")){
            // Cerrar la conexion con el cliente (el hilo en cuestion cierra la conexion)
            printf("Closing connection with client...\n");
            close(varClientSocket);
        }
        printf("Closing connection...\n");
    }
    // Cerrar el socket original para liberar el puerto
    close(varServerSocket);

    exit(EXIT_SUCCESS);
}


void handle_sigterm(int atrSig){
    printf("SIGTERM Recibida! %d\n", atrSig);
    atrFinished = 1;
    //Cerrar todos los recursos abiertos
    fclose(stdin);
}