#ifndef KBUF_H
#define KBUF_H

/** @brief Buffer de memoria reservado dinamicamente
* Se reserva el conjunto de paginas requerido para contener
* esta estructura de datos y las estructuras del tamaño requerido por el usuario.
* La estructura de datos se ubica al principio de la memoria reservada.
* El area efectiva del buffer se encuentra despues de la estructura de datos.
*/
typedef struct kbuf {
char * data; /**< Apuntador al buffer usable */
struct kitem * free_list; /** Apuntador a la cabeza de la lista de elementos disponibles */
unsigned int elemsize; /**< Tamaño de cada elemento en el buffer solicitado por el usuario */
unsigned int size; /**< Tamaño real que ocupa cada elemento del buffer */
unsigned int free; /**< Cantidad de elementos libres dentro del buffer */
unsigned int total; /**< Cantidad de elementos de elemsize que almacena el buffer */
unsigned short pages; /**< Cantidad de paginas que ocupa el buffer + esta estructura de datos */
}kbuf;

/** @brief Item dentro de un buffer. */
typedef struct kitem {
struct kitem * next;
//.... Al inicio de cada ítem se almacena un apuntador al siguiente ítem
//disponible.
//Esta estructura ocupa mínimo 4 bytes (el tamaño de un apuntador)
}kitem;

/**
 * @brief Crea un nodo en la direccion especificada
 * 
 * @param prmDireccion  Direcciond memoria a especificar
 * @return kitem* 
 */
kitem *crearNodo(unsigned int prmDireccion);

/** @brief Crea un buffer de elementos
*
* @param size Tamaño de un elemento del buffer
* @return kpool* Referencia al nuevo buffer
*/
kbuf * kbuf_create(unsigned int elemsize);

/** @brief Reserva un elemento dentro de un buffer
* @param b Buffer
* @return void* Apuntador al elemento dentro del buffer
*/
void * kbuf_allocate(kbuf * b);

/** @brief Libera un elemento dentro de un buffer
* @param b Buffer
* @param ptr Apuntador
*/
void kbuf_free(kbuf * b, void * ptr);

/** @brief Libera la memoria asociada con un Buffer
*
* @param b Buffer
*/
void kbuf_destroy(kbuf * b);

/** @brief Retorna la cantidad de elementos disponibles en el buffer
* @param b Buffer
* @return Cantidad de elementos disponibles
*/
int kbuf_available(kbuf * b);

/** @brief Verifica si el buffer se encuentra lleno
* @param b Buffer
* @return 1 si el buffer se encuentra lleno, 0 en caso contrario
*/
int kbuf_full(kbuf * b);

/** @brief Verifica si el buffer se encuentra vacio
* @param b Buffer
* @return 1 si el buffer se encuentra vacio, 0 en caso contrario
*/
int kbuf_empty(kbuf * b);

/** @brief Verifica si una direccion se encuentra dentro del buffer
* @param b Buffer
* @param ptr Direccion a verificar
* @return 1 si la direccion se encuentra dentro del buffer
*/
int kbuf_contains(kbuf * b, void * ptr);

#endif