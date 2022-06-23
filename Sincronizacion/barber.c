/**
 * @file barber.c
 * @author Darwin Latorre, Juan Ante
 * Implementacion de algoritmo del Barbero Dormilon
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sem.h"

/**
 * @brief Hilo barbero
 * @param prmArg Identificador del lector (int *)
 * @return NULL
 */
void *barber(void *prmArg);

/**
 * @brief Hilo cliente
 * @param prmArg Identificador del lector (int *)
 * @return NULL
 */
void *customer(void *prmArg);

void cut_hair();
void get_haircut(int prmIndex);

semaphore atrCustomers;
semaphore atrBarber;
semaphore atrMutex;

pthread_t thread_barbers;    // Arreglo de ID de hilo para los barberos pthread_create
pthread_t *thread_customers; // Arreglo de ID de hilo para los clientes pthread_create

int *atrID_customers; // Arreglo de "prmIndex" para los clientes

int atrFinished; // Indicador de fin de la ejecicion
int atrWaiting = 0;
int atrChairs = 1;

/**
 * @brief main
 * @param argc = 2
 * @param argv[0] = Ejecutable
 * @param argv[1] = Numero de clientes
 * @return int
 */
int main(int argc, char *argv[])
{
    int varNumClients;
    int varIndex;

    atrFinished = 0;

    // Crear los hilos
    if (argc != 2)
    {
        fprintf(stderr, "Deberia especificar el numero de clientes > 100\n");
        exit(EXIT_FAILURE);
    }

    varNumClients = 0;

    varNumClients = atoi(argv[1]);

    if (varNumClients < 100)
    {
        fprintf(stderr, "Deberia especificar el numero de clientes > 100\n");
        exit(EXIT_FAILURE);
    }

    // Reservar memoria para los ID y para los "varIndex" de los clientes
    thread_customers = (pthread_t *)malloc(varNumClients * sizeof(pthread_t));
    atrID_customers = (int *)malloc(varNumClients * sizeof(int));

    // Inicializar los semaforos
    sem_init(&atrCustomers, 0, 0);
    sem_init(&atrBarber, 0, 0);
    sem_init(&atrMutex, 0, 1);

    // Crear los hilos barberos
    // Crear un barbero, pasando como paramentro su ID (del arreglo de ID)
    pthread_create(&thread_barbers, NULL, barber, NULL);

    // Crear los hilos customers
    for (varIndex = 0; varIndex < varNumClients; varIndex++)
    {
        atrID_customers[varIndex] = varIndex + 1;
        // Crear un cliente, pasando como paramentro su ID (del arreglo de ID)
        pthread_create(&thread_customers[varIndex], NULL, customer, (void *)&atrID_customers[varIndex]);
    }

    // Esperar que los hilos terminen
    for (varIndex = 0; varIndex < varNumClients; varIndex++)
    {
        pthread_join(thread_customers[varIndex], NULL);
    }

    atrFinished = 1;
    
    pthread_join(thread_barbers, NULL);

    exit(EXIT_SUCCESS);
}

void *barber(void *prmArg)
{
    while (!atrFinished)
    {
        down(&atrCustomers);
        down(&atrMutex);
        atrWaiting = atrWaiting - 1;
        up(&atrBarber);
        up(&atrMutex);
        cut_hair();
    }
}
void *customer(void *prmArg)
{
    int varIndex;
    varIndex = *(int *)prmArg; // Recupera el ID del cliente

    down(&atrMutex);
    if (atrWaiting < atrChairs)
    {
        atrWaiting = atrWaiting + 1;
        up(&atrCustomers);
        up(&atrMutex);
        down(&atrBarber);
        get_haircut(varIndex);
    }
    else
        up(&atrMutex);
}

void cut_hair()
{
    printf("The barber has cut the hair\n");
    fflush(stdout);
}
void get_haircut(int prmIndex)
{
    printf("Customer %d wants a haircut\n", prmIndex);
    fflush(stdout);
}