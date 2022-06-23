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
    atrAddress.sin_port = htons(1046);//TODO Recibir el puerto a escuchar por la linea de comandos
    //inet_aton("0.0.0.0", &atrAddress.sin_addr);
    atrAddress.sin_addr.s_addr = INADDR_ANY; //0.0.0.0

    //Asociar el socket a una direccion (IPV4)
    bind(atrServerSocket, (struct sockaddr *)&atrAddress, sizeof(struct sockaddr_in));

    //Socket disponible
    listen(atrServerSocket, 10);

    //el sistema "llena" atrClientAddress con la informacion del cliente
    //y almacena en atrClientAddressLength el tamaño obtenido de esa direccion
    struct sockaddr_in atrClientAddress;
    memset(&atrClientAddress, 0, sizeof(struct sockaddr_in));
    socklen_t atrClientAddressLength;
    atrClientAddressLength = sizeof(struct sockaddr_in); //Tamaño esperado de la direccion

    //Aceptar la conexion
    atrClientSocket = accept(atrServerSocket, (struct sockaddr *)&atrClientAddress, &atrClientAddressLength);

    //TODO Comunicacion
    //Crear un hilo pasandole como parametro atrClientSocket (almacenado en un "arreglo")

    sleep(5);

    //Cerrar la conexion con el cliente (el hilo en cuestion cierra la conexion)
    close(atrServerSocket);

    exit(EXIT_SUCCESS);
}