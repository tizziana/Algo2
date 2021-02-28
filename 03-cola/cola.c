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
	cola_t* cola = malloc(sizeof(cola_t));
	if (cola == NULL) {
		return NULL;
	}
	cola->primero = NULL;
	cola->ultimo = NULL;
	return cola;
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

bool cola_esta_vacia(const cola_t *cola){
	return cola->primero == NULL;
}

void* cola_ver_primero(const cola_t *cola){
	if (cola_esta_vacia(cola)){
		return NULL;
	}
	return cola->primero->dato;
}

bool cola_encolar(cola_t *cola, void* valor){
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if (!nodo){
		return false;
	}
	nodo->dato = valor;
	nodo->prox = NULL;

	if (cola_esta_vacia(cola)){
		cola->primero = nodo;
	}
	else{
		cola->ultimo->prox = nodo;
	}
	cola->ultimo = nodo;
	return true;
}

void* cola_desencolar(cola_t *cola){
	if (cola_esta_vacia(cola)){
		return NULL;
	}
	void* desencolado = cola->primero->dato;
	cola_t* referencia_al_nodo = cola->primero->prox;
	free(cola->primero);
	cola->primero = referencia_al_nodo;

	return desencolado;
}