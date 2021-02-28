#include "pila.h"
#include <stdlib.h>
#include <stdbool.h>
#define TAMANIO_INICIAL 2000
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
	pila_t* pila = malloc(sizeof (pila_t));
	if (pila==NULL) return NULL;
	pila->datos = malloc (TAMANIO_INICIAL *sizeof( void*));
	if (pila->datos==NULL && TAMANIO_INICIAL >0) {
		free(pila);
		return NULL;
	}
	pila -> cantidad = 0;
	pila->capacidad = TAMANIO_INICIAL;
	return pila;
}


void pila_destruir(pila_t *pila){
	free(pila->datos);
	free (pila);
}

bool pila_esta_vacia(const pila_t *pila){
	return (pila-> cantidad ==0) ;
}

bool modificar_tamanio(pila_t*pila, size_t capacidad_nueva){
	pila ->capacidad = capacidad_nueva;
	void* pila_nueva = realloc (pila->datos ,sizeof(void*) * capacidad_nueva );
	if (pila_nueva==NULL) return false;
	pila->datos= pila_nueva;
	return true;
}

bool pila_apilar(pila_t *pila, void* valor){
	if (pila->cantidad == pila-> capacidad){   
		if (! modificar_tamanio(pila, pila-> capacidad*2)) return false;	
	}
	pila -> datos [pila ->cantidad] = valor;
	pila->cantidad ++;
	return true;
		
}
void* pila_ver_tope(const pila_t *pila){
	if (pila_esta_vacia (pila)) return NULL;
	return (pila->datos[pila ->cantidad-1]);
}

void* pila_desapilar(pila_t *pila){
	if (pila_esta_vacia(pila)) return NULL;
	if (pila->cantidad == pila->capacidad / 4 && pila->capacidad > TAMANIO_INICIAL) {
		modificar_tamanio(pila, pila->capacidad / 2);
	}
	void * nueva_pila = pila->datos[pila->cantidad-1];
	pila->cantidad--;
	return nueva_pila;
}

