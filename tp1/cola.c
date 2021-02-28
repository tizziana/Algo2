#include "cola.h"
#include <stdlib.h>


typedef struct nodo{
	void* dato;
	struct nodo *prox;
}nodo_t;

struct cola{
	nodo_t* primero;
	nodo_t* ultimo;
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA COLA
 * *****************************************************************/

cola_t* cola_crear(void) {
	cola_t* cola = calloc(1,sizeof(cola_t));
	if (cola == NULL) {
		return NULL;
	}
	return cola;
}

bool cola_esta_vacia(const cola_t *cola){
	return cola->primero == NULL;
}

void* cola_ver_primero(const cola_t *cola){
	if (cola_esta_vacia(cola)) return NULL;
	return cola->primero->dato;
}

nodo_t* nodo_crear(void* valor){
	nodo_t* nuevo_nodo = malloc(sizeof(nodo_t));
	if (!nuevo_nodo) return NULL;
	
	nuevo_nodo->dato = valor;
	nuevo_nodo->prox = NULL;

	return nuevo_nodo;
}

bool cola_encolar(cola_t *cola, void* valor){
	nodo_t* nodo = nodo_crear(valor);
	if (nodo == NULL) return false;

	nodo->dato = valor;
	nodo->prox = NULL;

	if (cola_esta_vacia(cola)) cola->primero = nodo;
	else cola->ultimo->prox = nodo;

	cola->ultimo = nodo;
	return true;
}

void* cola_desencolar(cola_t *cola){
	if (cola_esta_vacia(cola)) return NULL;

	nodo_t *desencolado = cola->primero;
	void* devolucion = desencolado->dato;
	if (cola->primero == cola->ultimo){
		cola->primero = NULL;
		cola->ultimo = NULL;
	}
	else cola->primero = cola->primero->prox;
	free(desencolado);
	return devolucion;
}

void cola_destruir(cola_t *cola, void destruir_dato(void*)){
	while (!cola_esta_vacia(cola)){
		void* dato_a_destruir = cola_desencolar(cola);
		if(destruir_dato){
		destruir_dato(dato_a_destruir);
		}
	}
	free(cola);
}
