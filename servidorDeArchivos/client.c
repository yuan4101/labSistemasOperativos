#include "protocol.h"
#include "split.h"

int atrFinished;

/**
 * @brief Funcion que maneja el SIGTERM
 * @param int De la señal recibida
 */
void handler_sigterm(int);

int main(int argc, char *argv[])
{

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
    if (argc != 2)
    {
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
        split_list *varSplitList;
        varSplitList = split(varMensaje, " \n\r\t");

        // Verificar los parametros
        if (EQUALS(varSplitList->parts[0], "exit"))
        {
            varSplitList->parts[1] = "0";
            atrFinished = 1;
        }
        else if (varSplitList->count == 1 || varSplitList->count > 2)
        {
            fprintf(stderr, "Debe especificar el comando y el nombre del archivo.\n");
            continue;
        }

        // Enviar request
        request varRequest;
        strcpy(varRequest.atrOperation, varSplitList->parts[0]);
        strcpy(varRequest.atrFileName, varSplitList->parts[1]);

        if (EQUALS(varRequest.atrOperation, "get"))
        {
            write(varServerSocket, (char *)&varRequest, sizeof(request));
            // TODO: Recibir archivo
            // 2. Recibir la informacion del archivo del cliente
            // 3. Crear la ruta "Files/FILENAME"
            // 3.1 Abrir el archivo en modo escritura
            // 3.2 Leer una parte del socket
            // 3.3 Escribir la parte en el archivo
            // 3.4 Repetir 3.2 mientras falte por leer
        }
        else if (EQUALS(varRequest.atrOperation, "put"))
        {
            file_info varFileInfo;
            memset(&varFileInfo, 0, sizeof(file_info));
            int fd;
            struct stat file_stat;
            int len;
            int offset = 0;
            int sent_bytes = 0;
            int remain_data;
            char file_size[256];

            // TODO: Enviar arhivo
            // 1. Verificar si el archivo existe
            varFileInfo = read_file_atr(varRequest.atrFileName);
            // 2. Verificar informacion del archivo (atrSize = -1) si el archivo no existe
            if (varFileInfo.atrSize >= 0)
            {
                write(varServerSocket, (char *)&varRequest, sizeof(request));
                fd = open(varFileInfo.atrFileName, O_RDONLY);
                fstat(fd, &file_stat);
                printf("File Size: \n%d bytes\n", file_stat.st_size);
                sprintf(file_size, "%d", file_stat.st_size);
                len = send(varServerSocket, file_size, sizeof(file_size), 0);
            }
            offset = 0;
            remain_data = file_stat.st_size;
            /* Sending file data */
            while (((sent_bytes = sendfile(varServerSocket, fd, &offset, BUFSIZ)) > 0) && (remain_data > 0))
            {
                fprintf(stdout, "1. Server sent %d bytes from file's data, offset is now : %d and remaining data = %d\n", sent_bytes, offset, remain_data);
                remain_data -= sent_bytes;
                fprintf(stdout, "2. Server sent %d bytes from file's data, offset is now : %d and remaining data = %d\n", sent_bytes, offset, remain_data);
            }
            fprintf(stderr, "El archivo no existe.\n");
        }
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