#ifndef LISTA_H
#define LISTA_H
#include <stdio.h>
#include <stdbool.h>

//Definicion de los tipos de datos

typedef struct lista lista_t;
typedef struct lista_iter lista_iter_t; //El iterador esta planteado como una lista de punteros genericos.

//Primitivas de la Lista Enlazada

// Crea una lista.
// Post: devuelve una nueva lista vacía.
lista_t *lista_crear(void);

// Devuelve verdadero o falso, según si la lista tiene o no elementos enlazados.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento al principio de la lista. 
// Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, valor se encuentra al principio.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Agrega un nuevo elemento a la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, valor se encuentra al final.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Si la lista tiene elementos, se quita el primer elemento de la lista,
//  y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devuelve el valor del primer elemento, la lista
// contiene un elemento menos, en el caso de no estar vacía.
void *lista_borrar_primero(lista_t *lista);


// Si la lista tiene elementos, se devuelve el valor del primero,
// si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devuelve el primer elemento de la lista, cuando no está vacía.
void *lista_ver_primero(const lista_t *lista);

// Si la lista tiene elementos, se devuelve el valor del ultimo,
// si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devuelve el ultimo elemento de la lista, cuando no está vacía.
void *lista_ver_ultimo(const lista_t* lista);

//Devuelve el largo de la lista.
//Pre: la lista fue creada.
int lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *));



//Primitivas de iterador externo.

// Crea un iterador.
// Pre: La lista fue creada.
// Post: devuelve un nuevo iterador que esta apuntando al primer elemento de
// la lista.
lista_iter_t *lista_iter_crear(lista_t *lista);

// Avanza al siguiente elemento de la lista.
// Pre: el iterador fue creada.
// Post: Devuelve true si se avanzo al siguiente elemento de la lista y false
// si la lista esta vacia o ya se encontraba en su ultimo elemento.
bool lista_iter_avanzar(lista_iter_t *iter);

// Si la lista tiene elementos, se devuelve el valor del actual,
// si está vacía devuelve NULL.
// Pre: el iterador fue creada.
// Post: se devuelve el elemento actual (el nodo que esta apuntando
// el iterador en ese momento), si esta al final devuelve NULL.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Verifica que el iterador se encuentre al final.
// Pre: el iterador fue creado.
// Post: Devuelve true si se encuentra al final de la lista y false
// en el caso contrario.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador.
// Pre: el iterador fue creado.
// Post: se elimina el iterador.
void lista_iter_destruir(lista_iter_t *iter);

// Inserta un nuevo elemento en la lista en la posicion actual del iterador.
// Pre: el iterador fue creado.
// Post: Devuelve true si se inserto el nuevo elemento en la posicion actual
// del iterador y false en el caso de que no se haya podido insertar. El 
// iterador queda en la misma posicion. 
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Elimina el elemento de la lista que se encuentre en la posicion actual
// del iterador.
// Pre: el iterador fue creado.
// Post: elimina y devuelve el valor del nodo que se encontraba en la posicion
// actual del iterador. El iterador queda en la posicion siguiente a la 
// que ocupaba el dato borrado.
void *lista_iter_borrar(lista_iter_t *iter);

//Primitiva del iterador intero

// A cada uno de los elementos de la lista le aplico la funcion visitar.
// Pre: la lista fue creada. Visitar es una funcion que recibe un dato y un extra y 
// devuelve true si visito o false en el caso contrario.
// Post: se le aplica la funcion a todos los elementos
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);



#endif // LISTA_H
