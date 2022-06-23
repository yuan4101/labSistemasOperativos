/**
 * @file sem.h
 * @author Darwin Latorre, Juan Ante
 * Funciones auxiliares para uso de semaforos
 */

#ifndef SEM_H
#define SEM_H

#include <semaphore.h>

/**
 * @brief definicion del tipo semaforo
 * 
 */
typedef sem_t semaphore;

/**
 * @brief Bloquea un semaforo
 * 
 * @param prmSemaforoBloquable semaforo a bloquear
 */
void down(semaphore * prmSemaforoBloquable);

/**
 * @brief Desbloquea un semaforo
 * 
 * @param prmSemaforoDebloqueable semaforo a desbloquear
 */
void up(semaphore * prmSemaforoDebloqueable);



#endif