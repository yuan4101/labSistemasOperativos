/**
 * @file readwrite.c
 * @author Darwin Latorre, Juan Ante
 * Implementacion de algoritmo de los Lectores - Escritores
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sem.h"

/**
 * @brief Hilo lector
 * @param prmArg Identificador del lector (int *)
 * @return NULL
 */
void * reader(void * prmArg);

/**
 * @brief Hilo escritor
 * @param prmArg Identificador del escritor (int *)
 * @return NULL
 */
void * writer(void * prmArg);

void read_data_base(int prmIndex);
void use_data_read(int prmIndex);
void think_up_data(int prmIndex);
void write_data_base(int prmIndex);

semaphore atrMutex;
semaphore atrDB;

pthread_t * thread_readers; //Arreglo de ID de hilo para los lectores pthread_create
pthread_t * thread_writers; //Arreglo de ID de hilo para los escritores pthread_create

int * atrID_readers; //Arreglo de "prmIndex" para los lectores
int * atrID_writers; //Arreglo de "prmIndex" para los escritores

int atrFinished; //Indicador de fin de la ejecicion
int rc = 0;

/**
 * @brief main
 * @param argc = 3
 * @param argv[0] = Ejecutable
 * @param argv[1] = Numero de lectores
 * @param argv[2] = Numero de escritores
 * @return int 
 */
int main(int argc, char *argv[])
{
    int varNumReaders;
    int varNumWriters;
    int varIndex;

    atrFinished = 0;

    //Crear los hilos
    if (argc != 3)
    {
        fprintf(stderr, "Deberia especificar el numero de lectores > 50 y de escritores > 20\n");
        exit(EXIT_FAILURE);
    }
    
    varNumReaders = 0;
    varNumWriters = 0;
    
    varNumReaders = atoi(argv[1]);
    varNumWriters  = atoi(argv[2]);

    if (varNumReaders < 50 || varNumWriters < 20)
    {
        fprintf(stderr, "Deberia especificar el numero de lectores > 50 y de escritores > 20\n");
        exit(EXIT_FAILURE);
    }
    
    //Reservar memoria para los ID y para los "varIndex" de los lectores
    thread_readers = (pthread_t *)malloc(varNumReaders * sizeof(pthread_t));
    atrID_readers = (int*)malloc(varNumReaders * sizeof(int));
    
    //Reservar memoria para los ID y para los "varIndex" de los escritores
    thread_writers = (pthread_t *)malloc(varNumWriters * sizeof(pthread_t));
    atrID_writers = (int*)malloc(varNumWriters * sizeof(int));
    
    //Inicializar los semaforos
    sem_init(&atrMutex, 0, 1);
    sem_init(&atrDB, 0, 1);

    //Crear los hilos lectores
    for (varIndex = 0; varIndex < varNumReaders; varIndex++)
    {
        atrID_readers[varIndex] = varIndex + 1;
        //Crear un lector, pasando como paramentro su ID (del arreglo de ID)
        pthread_create(&thread_readers[varIndex], NULL, reader, (void *) &atrID_readers[varIndex]);
    }
    //Crear los hilos escritores
    for (varIndex = 0; varIndex < varNumWriters; varIndex++)
    {
        atrID_writers[varIndex] = varIndex + 1;
        //Crear un escritor, pasando como paramentro su ID (del arreglo de ID)
        pthread_create(&thread_writers[varIndex], NULL, writer, (void *) &atrID_writers[varIndex]);
    }

    //Bloquear el hilo principal para permitir la ejecicion de los hilos
    sleep(10);
    
    atrFinished = 1;

    //Esperar que los hilos terminen 
    for (varIndex = 0; varIndex < varNumReaders; varIndex++)
    {
        pthread_join(thread_readers[varIndex], NULL);
    }
    for (varIndex = 0; varIndex < varNumWriters; varIndex++)
    {
        pthread_join(thread_writers[varIndex], NULL);
    }

    exit(EXIT_SUCCESS);
}

void * reader(void * prmArg)
{
    int varIndex;
    varIndex = *(int* )prmArg; //Recupera el ID del lector

    while (!atrFinished)
    {
        down(&atrMutex);
        rc = rc + 1;

        if (rc == 1)
            down(&atrDB);

        up(&atrMutex);
        read_data_base(varIndex);
        down(&atrMutex);
        rc = rc - 1;

        if (rc == 0)
            up(&atrDB);

        up(&atrMutex);
        use_data_read(varIndex);
    }
}
void * writer(void * prmArg)
{
    int varIndex;
    varIndex = *(int *)prmArg; //Recupera el ID del escritor

    while (!atrFinished)
    {
        think_up_data(varIndex);
        down(&atrDB);

        write_data_base(varIndex);
        up(&atrDB);
    }
}

void read_data_base(int prmIndex) {
    printf("Reader %d reading database\n", prmIndex);
    fflush(stdout);
    usleep(rand() % 1000);
}
void use_data_read(int prmIndex) {
    printf("Reader %d using data read\n", prmIndex);
    fflush(stdout);
    usleep(rand() % 2000);
}
void think_up_data(int prmIndex) {
    printf("Writer %d thinking up data\n", prmIndex);
    fflush(stdout);
    usleep(rand() % 5000);
}
void write_data_base(int prmIndex) {
    printf("Writer %d writing the database\n", prmIndex);
    fflush(stdout);
    usleep(rand() % 3000);
}