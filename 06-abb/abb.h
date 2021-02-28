#ifndef ABB_H
#define ABB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* *****************************************************************
 *                     		ESTRUCTURAS
 * *****************************************************************/

typedef struct abb abb_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);

typedef struct abb_iter abb_iter_t;


/* *****************************************************************
 *                  	 PRIMITIVAS DEL ABB
 * *****************************************************************/

// Crea un arbol de busqueda binaria.
// Post: devuelve un arbol vacio.
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

// Agrega un nuevo nodo al arbol como una hoja. Devuelve falso en caso de error.
// Pre: el arbol fue creado.
// Post: se agregó un nuevo nodo al arbol, el dato se encuentra como una hoja
// del arbol.
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

// Saca un nodo del arbol. Si el arbol tiene elementos, se quita el nodo y
// se devuelve el dato. Si está vacío, devuelve NULL.
// Pre: el arbol fue creado.
// Post: se devolvió el dato del nodo borrado, el arbol
// contiene un nodo menos, si no estaba vacío.
void *abb_borrar(abb_t *arbol, const char *clave);

// Obtiene el dato del nodo que tiene como clave la que se paso por parametro. Si el
// arbol tiene nodos se devuelve el dato de la clave ingresada, si está vacía devuelve NULL.
// Pre: el arbol fue creado.
// Post: se devolvió el dato de la clave ingresada, en caso de encontrarse en el arbol.
void *abb_obtener(const abb_t *arbol, const char *clave);

// Devuelve true en caso de que se encuentre, en el arbol, la clave ingresada. Devuelve
// false en caso contrario. 
// Pre: el arbol fue creado.
// Post: se devolvió true o false si la clave se encuentra o no en el arbol.
bool abb_pertenece(const abb_t *arbol, const char *clave);

// Devuelve la cantidad de nodos del arbol.
// Pre: el arbol fue creado.
// Post: se devolvio la canridad de nodos del arbol.
size_t abb_cantidad(abb_t *arbol);

// Destruye el arbol.
// Pre: el arbol fue creado.
// Post: se eliminaron todos los elementos de la lista.
void abb_destruir(abb_t *arbol);

/* *****************************************************************
 *             	  PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

// Crea un iterador de abb.
// Pre: el arbol fue creado.
// Post: devuelve un iterador en la posicion inicial.
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

// Avanza un nodo del abb.
// Pre: el iterador fue creado.
// Post: devuelve true si pudo avanzar, false en caso contrario.
bool abb_iter_in_avanzar(abb_iter_t *iter);

// Pre: el iterador fue creado.
// Post: devuelve la clave correspondiente a la posicion actual
// del iterador.
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

// Informa si se esta o no al final del abb.
// Pre: el iterador fue creado.
// Post: devuelve true si se esta al final del abb, y false en caso 
// contrario.
bool abb_iter_in_al_final(const abb_iter_t *iter);

// Destruye el iterador.
// Pre: el iterador fue creado.
void abb_iter_in_destruir(abb_iter_t* iter);

/* *****************************************************************
 *             	  PRIMITIVA DEL ITERADOR INTERNO
 * *****************************************************************/

// Itera un arbol binario de forma "inorder".
// Pre: el abb fue creado, la primitiva recibe la funcion "visitar"
// que devuelve true si se debe seguir iterando o false en caso contrario.
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

#endif