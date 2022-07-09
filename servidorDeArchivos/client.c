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

            file_info varFileInfo;
            memset(&varFileInfo, 0, sizeof(file_info));
            read(varServerSocket, (char *)&varFileInfo, sizeof(file_info));

            printf("Nombre %s, size %d\n", varFileInfo.atrFileName, varFileInfo.atrSize);

            FILE * varDestinyFilePointer;
            varDestinyFilePointer = fopen(varFileInfo.atrFileName, "w+");

            int varRemainData = varFileInfo.atrSize;
            
            ssize_t varLen;
            char varBuffer[BUFSIZ];

            while ((varRemainData > 0) && ((varLen = read(varServerSocket, varBuffer, BUFSIZ)) > 0))
            {
                fwrite(varBuffer, sizeof(char), varLen, varDestinyFilePointer);
                varRemainData -= varLen;
                printf("Recibe %d bytes y en espera :- %d bytes\n", varLen, varRemainData);
            }
            fclose(varDestinyFilePointer);
        }
        else if (EQUALS(varRequest.atrOperation, "put"))
        {
            file_info varFileInfo;
            memset(&varFileInfo, 0, sizeof(file_info));

            int varfileDescriptor;
            int remainData;
            int sentBytes = 0;

            varFileInfo = read_file_atr(varRequest.atrFileName);

            if (varFileInfo.atrSize >= 0)
            {
                write(varServerSocket, (char *)&varRequest, sizeof(request));
                write(varServerSocket, (char *)&varFileInfo, sizeof(file_info));
                varfileDescriptor = open(varFileInfo.atrFileName, O_RDONLY);
                printf("File Size: %d bytes\n", varFileInfo.atrSize);

                remainData = varFileInfo.atrSize;

                while (((sentBytes = sendfile(varServerSocket, varfileDescriptor, NULL, BUFSIZ)) > 0) && (remainData > 0))
                {
                    printf("1. Server sent %d bytes from file's data, and remaining data = %d\n", sentBytes, remainData);
                    remainData -= sentBytes;
                    printf("2. Server sent %d bytes from file's data, and remaining data = %d\n", sentBytes, remainData);
                }
            }
            else{
                fprintf(stderr, "El archivo no existe.\n");
            }
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