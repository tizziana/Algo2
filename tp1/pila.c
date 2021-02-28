#include "pila.h"
#include <stdlib.h>
#define CAPACIDAD 15
#define AGRANDAR 2
#define ACHICAR 1/4

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

pila_t* pila_crear(void) {
	pila_t* pila = malloc(sizeof(pila_t));

	if (pila == NULL) {
		return NULL;
	}
	pila->capacidad = CAPACIDAD;
	pila->cantidad = 0;
	pila->datos = malloc(CAPACIDAD * sizeof(void*));

	if (pila->datos == NULL) {
		free(pila);
		return NULL;
	}

	return pila;
}

void pila_destruir(pila_t *pila) {
	free(pila->datos);
	free(pila);
}

bool pila_esta_vacia(const pila_t *pila) {
	return pila->cantidad == 0;
}

void* pila_ver_tope(const pila_t *pila) {

	if (pila_esta_vacia(pila)) {
		return NULL;
	}
	return pila->datos[pila->cantidad-1];
}

bool pila_redimensionar(pila_t* pila, size_t tam_nuevo) {
	void** datos_nuevo = realloc(pila->datos, tam_nuevo * sizeof(void*));

	if (datos_nuevo == NULL) {
		return false;
	}
	pila->datos = datos_nuevo;
	pila->capacidad = tam_nuevo;

	return true;
}

bool pila_apilar(pila_t *pila, void* valor) {

	if (pila->cantidad == pila->capacidad) {
		if (!pila_redimensionar(pila, pila->capacidad * AGRANDAR)) {
			return false;
		}
	}
	pila->datos[pila->cantidad] = valor;
	pila->cantidad++;

	if(pila_ver_tope(pila) != valor) {
		return false;
	}
	return true;
}

void* pila_desapilar(pila_t *pila){

	if(pila_esta_vacia(pila)) {
		return NULL;
	}

	void *tope = pila_ver_tope(pila);
	if (pila->cantidad <= pila->capacidad*ACHICAR) {
		pila_redimensionar(pila, pila->capacidad * ACHICAR);
	}
	pila->cantidad-=1;
	return tope;
}