#include "pila.h"
#include <stdlib.h>
#define CAPACIDAD_PILA 20
#define CONSTANTE_REDIMENSION 2
#define TAMANIO_REDUCCION 4

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

	pila->datos = malloc(CAPACIDAD_PILA * sizeof(void*));

	if (pila->datos == NULL) {
		free(pila);
		return NULL;
	}

	pila->capacidad = CAPACIDAD_PILA;
	pila->cantidad = 0;
	return pila;
}

void pila_destruir(pila_t *pila) {
	free(pila->datos);
	free(pila);
}

bool pila_esta_vacia(const pila_t *pila) {
	return pila->cantidad == 0;
}

//Redimensiona la pila. Recibe la pila y un numero que define si se aumenta o reduce el tamaño.
//Pre: el numero debe ser igual a alguna de las constantes globales AGRANDAR o ACHICAR y
//la pila ya existe.
//Post: se devuelve true si se logra redimensionar, false de lo contrario.
bool pila_redimensionar(pila_t* pila, size_t tamanio) {
	void** datos_nuevo = realloc(pila->datos, tamanio * sizeof(void*));
		
	if (datos_nuevo == NULL) {
		return false;
	}

	pila->datos = datos_nuevo;
	pila->capacidad = tamanio;

	return true;
}

bool pila_apilar(pila_t *pila, void* valor) {
	if (pila->cantidad == pila->capacidad) {
		if (!pila_redimensionar(pila, pila->capacidad * CONSTANTE_REDIMENSION)) {
			return false;
		}
	}

	pila->datos[pila->cantidad] = valor;
	pila->cantidad++;
	return true;
}

void* pila_ver_tope(const pila_t *pila) {
	if (pila_esta_vacia(pila)) {
		return NULL;
	}
	
	return pila->datos[pila->cantidad - 1];
}

void* pila_desapilar(pila_t *pila) {
	if (pila_esta_vacia(pila)) {
		return NULL;
	}

	if (pila->cantidad <= pila->capacidad / TAMANIO_REDUCCION) {
		pila_redimensionar(pila, pila->capacidad / CONSTANTE_REDIMENSION);
	}

	pila->cantidad--;
	return pila->datos[pila->cantidad];
}