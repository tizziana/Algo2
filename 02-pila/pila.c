#include "pila.h"
#include <stdlib.h>

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
	void** datos;
	size_t cantidad;  // Cantidad de elementos almacenados.
	size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t* pila_crear(void){

	pila_t* pila = malloc(sizeof(pila_t));

	if (pila == NULL) {
		return NULL;
	}
	pila->capacidad = 6;
	pila->cantidad = 0;
	pila->datos = malloc(pila->capacidad * sizeof(void*));

	if (pila->datos == NULL) {
		free(pila);
		return NULL;
	}

	return pila;
}


void pila_destruir(pila_t *pila){

	free(pila->datos);
	free(pila);
}


bool pila_esta_vacia(const pila_t *pila){

	if (pila->cantidad == 0){
		return true;
	}
	return false;
}


bool pila_apilar(pila_t *pila, void* valor){

	if (pila->cantidad == pila->capacidad){
		void *nueva_capacidad = realloc(pila->datos, (2 * pila->capacidad) * sizeof(void*));

		if (nueva_capacidad == NULL) {
				return false;
		}
		pila->datos = nueva_capacidad;
		pila->capacidad = (pila->capacidad)*2;
	}

	pila->datos[pila->cantidad] = valor;
	pila->cantidad++;
	return true;
}


void* pila_ver_tope(const pila_t *pila){

	if (pila_esta_vacia(pila)){
		return NULL;
	}
	return pila->datos[pila->cantidad-1];
}


void* pila_desapilar(pila_t *pila){

	if(pila_esta_vacia(pila)){
		return NULL;
	}
	
	void *tope = pila_ver_tope(pila);

	if (pila->cantidad <= pila->capacidad/4){
		void* nueva_capacidad = realloc(pila->datos, (pila->capacidad / 2) * sizeof(void*));
		pila->datos = nueva_capacidad;
		pila->capacidad = (pila->capacidad)/2;
	}
	pila->cantidad-=1;

	return tope;
}
