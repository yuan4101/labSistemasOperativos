#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "protocol.h"
#include "split.h"

int atrFinished;

/**
 * @brief Funcion que maneja el SIGTERM
 * @param int De la señal recibida
 */
void handler_sigterm(int);


#include "protocol.h"

int main(int argc, char * argv[]){

    // #####################   SIGTERM   #########################

    struct sigaction varAct;
    struct sigaction varOldAct;
    memset(&varAct, 0, sizeof(struct sigaction));
    memset(&varOldAct, 0, sizeof(struct sigaction));

    // Cuando se reciba SIGTERM se ejecutara handler_sigterm
    varAct.sa_handler = handler_sigterm;

    // Instalamos el navegador para SIGTERM
    sigaction(SIGTERM, &varAct, &varOldAct);

    // ###########################################################

    // Lectura de puerto desde consola
    if (argc != 2) {
        fprintf(stderr, "Debe especificar el puerto del cliente.\n");
        exit(EXIT_FAILURE);
    }
    int varPortServer = 0;
    varPortServer = atoi(argv[1]);

    // Socket del servidor
    int varServerSocket;

    // Direccion IPV4
    struct sockaddr_in varAddress;

    // Socket IPv4, de tipo flujo (stream)
    varServerSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Preparar la direccion para asociarla al socket
    memset(&varAddress, 0, sizeof(struct sockaddr_in));
    varAddress.sin_family = AF_INET;
    varAddress.sin_port = htons(varPortServer);
    varAddress.sin_addr.s_addr = INADDR_ANY;

    // Conectar
    connect(varServerSocket, (struct sockaddr *)&varAddress, sizeof(struct sockaddr));
    
    // Recibir mensaje de bienvenida del servidor
    int varNumBytes;
    char varMensaje[BUFSIZ];
    memset(&varMensaje, 0, BUFSIZ);
    varNumBytes = read(varServerSocket, varMensaje, sizeof(varMensaje));
    printf("Received: %s", varMensaje);

    atrFinished = 0;

    while (!atrFinished)
    {
        // Input from stdin
        printf("\nRequest\n> ");
        memset(&varMensaje, 0, BUFSIZ);
        fgets(varMensaje, BUFSIZ, stdin);

        // Dividir input
        split_list * varSplitList;
        varSplitList = split(varMensaje, " \n\r\t");


        // Verificar los parametros
        if (EQUALS(varSplitList->parts[0], "exit")) {
            varSplitList->parts[1] = "0";
            atrFinished = 1;
        } else if (varSplitList->count == 1 || varSplitList->count > 2) {
            fprintf(stderr, "Debe especificar el comando y el nombre del archivo.\n");
            continue;
        }

        // Enviar request
        request varRequest;
        strcpy(varRequest.atrOperation, varSplitList->parts[0]);
        strcpy(varRequest.atrFileName, varSplitList->parts[1]);
        write(varServerSocket, (char *)&varRequest, sizeof(request));
    }

    printf("\nClosing connection with server...\n");
    close(varServerSocket);

    printf("Closing client...\n");

    exit(EXIT_SUCCESS);
}

void handler_sigterm(int atrSig)
{
    printf("SIGTERM Recibida! %d\n", atrSig);
    atrFinished = 1;
    // Cerrar todos los recursos abiertos
    fclose(stdin);
}