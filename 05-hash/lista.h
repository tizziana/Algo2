#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stdio.h>


/* *****************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* La lista está planteada como una lista de punteros genéricos. */

struct lista;
typedef struct lista lista_t;

struct lista_iter;
typedef struct lista_iter lista_iter_t;


/* ******************************************************************
 *                   PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista.
// Post: devuelve una nueva lista vacía.
lista_t *lista_crear(void);

// Devuelve verdadero o falso, según si la lista tiene o no elementos.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento a la lista al inicio. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, el dato se encuentra al inicio
// de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Agrega un nuevo elemento a la lista al final. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, el dato se encuentra al final
// de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Saca el primer elemento de la lista. Si la lista tiene elementos, se quita el
// primero de la lista, y se devuelve el dato, si está vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el dato del primer elemento anterior, la lista
// contiene un elemento menos, si la lista no estaba vacía.
void *lista_borrar_primero(lista_t *lista);

// Obtiene el dato del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el dato del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void *lista_ver_primero(const lista_t *lista);

// Obtiene el dato del ultimo elemento de la lista. Si la lista tiene
// elementos, se devuelve el dato del ultimo, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el ultimo elemento de la lista, cuando no está vacía.
void *lista_ver_ultimo(const lista_t* lista);

// Devuelve el largo de la lista.
// Pre: la lista fue creada.
// Post: se devolvio la canridad de elementos de la lista.
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *));


/* ******************************************************************
 *                PRIMITIVAS DEL ITERADO INTERNO
 * *****************************************************************/
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);


/* ******************************************************************
 *                PRIMITIVAS DE ITERADOR EXTERNO
 * *****************************************************************/

// Crea un iterador.
// Post: devuelve un iterador.
lista_iter_t *lista_iter_crear(lista_t *lista);

// El iterador avanza una posicion en la lista.
// Pre: el iterador fue creado.
// Post: se devolvio true si pudo avanzar o false si llego al final de la lista.
bool lista_iter_avanzar(lista_iter_t *iter);

// Obtiene el dato actual al que apunta el iterador.
// Pre: el iterador fue creado.
// Post: se devolvio el dato apuntado por el iterador actualmente. Devuelve NULL
// si la lista esta vacia o esta en el final.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Verifica si se llego o no al final de la lista
// Pre: la lista fue creada
// Post: se devolvio true si se llego al final de la lista, false en caso contrario.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador.
// Pre: el iterador fue creado.
// Post: se elimino el iterador.
void lista_iter_destruir(lista_iter_t *iter);

// Agrega un nuevo elemento a la lista entre el dato actual por el iterado y 
//el elemento anterior. Devuelve falso en caso de error.
// Pre: el iterador fue creado.
// Post: se agregó un nuevo elemento a la lista, dato se encuentra en entre la posicion
// actual del iterador y el anterior en la lista.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);


// Elimina el dato apuntado por el iterador.Si el iterador actual no apunta a NULL, se quita el
// elemento indicado y se devuelve el dato, devuelve NULL en caso contrario.
// Pre: la lista fue creada, el iterador fue creado.
// Post: el elemento apuntado por el iterador fue eliminado.
void *lista_iter_borrar(lista_iter_t *iter);

/*-----------------------------------------------------------------------------
 *  PRIMITIVAS DEL ITERADOR INTERNO
 *-----------------------------------------------------------------------------*/

// Itera una posicion en la lista. La funcion "visitar" recibe el 
// dato y un puntero extra, y devuelve true si se debe seguir iterando,
// false en caso contrario.
// Pre: la lista fue creada.
void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra), void *extra);

/* *****************************************************************
 *                    PRUEBAS PARA LA PILA
 * *****************************************************************/

// Realiza pruebas sobre la implementación de la lista del alumno.
//
// Las pruebas deben emplazarse en el archivo ‘pruebas_alumno.c’, y
// solamente pueden emplear la interfaz pública tal y como aparece en lista.h
// (esto es, las pruebas no pueden acceder a los miembros del struct lista).
//
// Para la implementación de las pruebas se debe emplear la función
// print_test(), como se ha visto en TPs anteriores.
void pruebas_lista_alumno(void);

#endif // LISTA_H
