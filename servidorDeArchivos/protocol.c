#include "protocol.h"

file_info read_file_atr(char * prmFileName){
    
    struct stat varStat;

    if(stat(prmFileName, &varStat) < 0){
        perror("stat");
        exit(EXIT_FAILURE);
    }
    
    //Verifica si es un archivo regural
    if(!S_ISREG(varStat.st_mode)){
        printf("%s no es un archivo\n", prmFileName);
        exit(EXIT_FAILURE);
    }

    file_info varInfo;
    char filename[PATH_MAX];
    strcpy(filename, prmFileName);

    memset(&varInfo, 0, sizeof(file_info));
    varInfo.atrSize = varStat.st_size;
    strcpy(varInfo.atrFileNeme, basename(prmFileName));

    return varInfo;
}

void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(EXIT_FAILURE);
}