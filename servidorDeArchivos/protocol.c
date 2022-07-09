#include "protocol.h"

file_info read_file_atr(char * prmFileName){
    
    struct stat varStat;
    file_info varInfo;

    if(stat(prmFileName, &varStat) < 0){
        fprintf(stderr, "No existe el directorio");
        varInfo.atrSize = -1;
    }
    //Verifica si es un archivo regular
    if(!S_ISREG(varStat.st_mode)){
        fprintf(stderr, "%s no es un archivo\n", prmFileName);
        varInfo.atrSize = -1;
    }

    memset(&varInfo, 0, sizeof(file_info));
    varInfo.atrSize = varStat.st_size;
    strcpy(varInfo.atrFileName, prmFileName);

    return varInfo;
}

void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(EXIT_FAILURE);
}

