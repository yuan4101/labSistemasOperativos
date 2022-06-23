#include "sem.h"

void down(semaphore * prmSemaforoBloquable){
    sem_wait(prmSemaforoBloquable);
}

void up(semaphore * prmSemaforoDebloqueable){
    sem_post(prmSemaforoDebloqueable);
}