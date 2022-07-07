#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char * argv[]){
    //Socket del servidor
    int atrServerSocket;

    //Socket del cliente
    int atrClientSocket;

    //Direccion del servidor
    struct sockaddr_in atrAddress;

    //Socket IPv4, de tipo flujo (stream)
    atrServerSocket = socket(AF_INET, SOCK_STREAM, 0);

    //Preparar la direccion para asociarla al socket
    memset(&atrAddress, 0, sizeof(struct sockaddr_in));
    atrAddress.sin_family = AF_INET;
    atrAddress.sin_port = htons(1046); //TODO Recibir el puerto a escuchar por la linea de comandos
    //inet_aton("0.0.0.0", &atrAddress.sin_addr);
    atrAddress.sin_addr.s_addr = INADDR_ANY; //0.0.0.0

    //Conectar
    connect(atrServerSocket, (struct sockaddr *)&atrAddress, sizeof(struct sockaddr));
    int varNumBytes;
    char varBuffer[100];
    memset(&varBuffer, 0, sizeof(varBuffer));
    varNumBytes = recv(atrServerSocket, varBuffer, 100, 0);
    varBuffer[varNumBytes] = '\0';
    printf("Received: %s\n", varBuffer);
}