#include "kbuf.h"

#include <stdio.h>
#include <stdlib.h>


kbuf *kbuf_create(unsigned int prmElemSize)
{
    kbuf *varRetorno = NULL;
    kitem *varItem;

    unsigned int varTempElemsize;
    varTempElemsize = prmElemSize;
    unsigned int varTempSize;
    unsigned int varTempFree;
    unsigned int varTempTotal;
    unsigned short varTempPages;

    // (CHECK) 1. Redondear prmElemSize a la siguiente potencia de 2
    // (CHECK) 1.1 Para elemsize < 4, size = 4, para elemsize > 4, size = elemsize
    if (varTempElemsize < 4)
        varTempSize = 4;
    else
    {
        int varPotencia = 4;
        while (1)
        {
            if (varTempElemsize <= varPotencia)
            {
                varTempSize = varPotencia;
                break;
            }
            varPotencia *= 2;
        }
    }

    // (CHECK) 2. Calcular la cantidad de items que caben en una pagina
    // (CHECK) 2.1 El buffer se almacena al incio de la pagina, se resta esta
    // cantidad antes de calcular la cantidad de items.
    int varRealElemSize = varTempSize;
    int varLeftPagSize = (4096 - sizeof(kbuf));
    printf("Page remaining space: %d\n", varLeftPagSize);

    varTempTotal = varLeftPagSize / varTempElemsize;
    varTempFree = varTempTotal;
    varTempPages = 1;

    // 2.2 Para tamaño mayor o igual a 4k, se necesita una pagina adicional
    // para almacenar la estructura del buffer (paginas += 1)
    if (varRealElemSize >= varLeftPagSize)
    {
        varRealElemSize -= varLeftPagSize;

        int varPag = varRealElemSize / 4096;

        if (varRealElemSize % 4096 > 0)
            varPag += 1;

        varTempPages += varPag;
    }

    // 3. Reservar la memoria necesaria (malloc)
    char *varPuntero = (char *)malloc(varTempPages * 4096);
    
    //      INICIALIZAR EL BUFFER
    // Apuntar al inicio de la memoria reservada
    // En esta direccion se guarda el buffer
    varRetorno = (kbuf *)varPuntero;
    varRetorno->elemsize = varTempElemsize;
    varRetorno->size = varTempSize;
    varRetorno->free = varTempFree;
    varRetorno->total = varTempTotal;
    varRetorno->pages = varTempPages;
    // Apuntar a la direccion que le corresponde al primer item
    int varTamnItemBuff = 1;
    while (varTamnItemBuff*varTempSize < sizeof(kbuf))
        varTamnItemBuff ++;

    unsigned int varAddres = (unsigned int)varPuntero;
    unsigned int varStartAddres = varAddres + (varTamnItemBuff * varTempSize);
    varRetorno->data = (char *)varStartAddres;

    //varItem = (kitem *)(varDireccion + offset);
    varItem = (kitem *)(varStartAddres);
    varRetorno->free_list = varItem;
    unsigned int varTempAddres = varStartAddres + varTempSize;

    // Repetir cada item (hasta el penultimo) apunta a la direccion del siguiente item
    // El ultimo item tiene como siguiente NULL

    //varRetorno->free_list->next = (kitem *)(varTempAddres);


    return varRetorno;
}

kitem *crearNodo(unsigned int prmDireccion){
    kitem * varNuevoNodo;
    varNuevoNodo = (kitem *)(prmDireccion);
    varNuevoNodo->next = NULL;
    return varNuevoNodo;
}

void *kbuf_allocate(kbuf *prmBuf)
{
    kitem *varItem = NULL;

    // 1. Si la cantidad de items disponibles es 0, retornar NULL
    // 2. Tomar el apuntador a la cabeza de los items libres
    // 3. La nueva cabeza es el nodo siguiente al apuntador obtenido en el paso anterior
    // 4. Retornar el apuntador obtenido.

    return varItem;
}

void kbuf_free(kbuf *prmBuf, void *varDireccion)
{
    // 1. Comprobar que la direccion se encuentre dentro de la region de items del buffer
    // 2. Ajustar la direccion a un limite de item de ser necesario

    kitem *varItem;
    // 3. Apuntar el item a la direccion ajustada en el paso anterior
    // 3.1 Guardar en varItem->next la cabeza actual de la lista de items libres
    // 3.3 varItem se convierte en la nueva cabeza
}