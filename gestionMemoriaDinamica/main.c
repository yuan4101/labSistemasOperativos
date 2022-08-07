#include <stdio.h>
#include <stdlib.h>
#include "kbuf.h"

int main(int argc, char * argv[]){
    kbuf * varBuf;
    int varElemSize = 40;

    varBuf = kbuf_create(varElemSize);

    printf("Item size: %d\n", varElemSize);
    printf("ElemSize: %d\n", varBuf->elemsize);
    printf("Size: %d\n", varBuf->size);
    printf("Elements in one page: %d\n", varBuf->total);
    printf("Used pages: %d\n", varBuf->pages);
    printf("Allocated page: 0x%x\n", (unsigned int)varBuf);
    printf("Allocated: 0x%x\n", (unsigned int)kbuf_allocate(varBuf));
    printf("Allocated: 0x%x\n", (unsigned int)kbuf_allocate(varBuf));
    printf("Allocated: 0x%x\n", (unsigned int)kbuf_allocate(varBuf));
    printf("First item at: 0x%x\n", (unsigned int)varBuf->data);

    exit(EXIT_SUCCESS);
}