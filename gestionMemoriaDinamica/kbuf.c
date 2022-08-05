#include "kbuf.h"

//Temp
#include <stdio.h>
#include <stdlib.h>

kbuf * kbuf_create(unsigned int prmElemSize){
    kbuf * varRetorno = NULL;
    kitem * varItem;

    if (prmElemSize < 4)
        varRetorno->elemsize = 4;
    else
        varRetorno->elemsize = prmElemSize;
    
    /*
    for (int varIndice = 4; varIndice < 4096; varIndice*2)
    {
        if (prmElemSize <= varIndice){
            varRetorno->elemsize = varIndice;
            break;
        }  
    }
    */

    //varRetorno->total = 69;
    //varRetorno->data = NULL;

    // (CHECK) 1. Redondear prmElemSize a la siguiente potencia de 2
    //1.1 Para elemsize < 4, size = 4, para elemsize > 4, size = elemsize

    //2. Calcular la cantidad de items que caben en una pagina
    //2.1 El buffer se almacena al incio de la pagina, se resta esta
    //cantidad antes de calcular la cantidad de items.
    //2.2 Para tamaño mayor o igual a 4k, se necesita una pagina adicional
    //para almacenar la estructura del buffer (paginas += 1)
    //3. Reservar la memoria necesaria (malloc)
    //      INICIALIZAR EL BUFFER
    //Apuntar al inicio de la memoria reservada
    //varRetorno = (kbuf*)varDireccion;
    //En esta direccion se guarda el buffer
    //Apuntar a la direccion que le corresponde al primer item
    //varItem = (kitem *)(varDireccion + offset);
    //Repetir cada item (hasta el penultimo) apunta a la direccion del siguiente item
    //El ultimo item tiene como siguiente NULL
    
    return varRetorno;
}

void * kbuf_allocate(kbuf * prmBuf){
    kitem * varItem = NULL;

    //1. Si la cantidad de items disponibles es 0, retornar NULL
    //2. Tomar el apuntador a la cabeza de los items libres
    //3. La nueva cabeza es el nodo siguiente al apuntador obtenido en el paso anterior
    //4. Retornar el apuntador obtenido.

    return varItem;
}

void kbuf_free(kbuf * prmBuf, void * varDireccion){
    //1. Comprobar que la direccion se encuentre dentro de la region de items del buffer
    //2. Ajustar la direccion a un limite de item de ser necesario

    kitem * varItem;
    //3. Apuntar el item a la direccion ajustada en el paso anterior
    //3.1 Guardar en varItem->next la cabeza actual de la lista de items libres
    //3.3 varItem se convierte en la nueva cabeza
}