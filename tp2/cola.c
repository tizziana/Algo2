#include "cola.h"
#include <stdlib.h>
#include <stdbool.h>
typedef struct nodo{
	void* dato;
	struct nodo * prox;
}nodo_t;

struct cola{
	nodo_t* primero;
	nodo_t* ultimo;	
};

cola_t* cola_crear(void){
	cola_t* cola = malloc(sizeof(cola_t));
	if  (!cola) return NULL;
	cola->primero= NULL;
	cola->ultimo=NULL;
	return cola;
}
nodo_t* nodo_crear_cola (void* valor){
	nodo_t* nodo= malloc(sizeof (nodo_t));
	if (!nodo) return NULL;
	nodo->dato= valor;
	nodo->prox=NULL;
	return nodo;
}
 
bool cola_esta_vacia(const cola_t *cola){
	return (cola->primero==	NULL && cola-> ultimo== NULL);
}
bool cola_encolar(cola_t *cola, void* valor){
	nodo_t* nodo_nuevo =nodo_crear_cola (valor);
	if (!nodo_nuevo) return NULL;
	if (cola_esta_vacia(cola)){
		cola->primero= nodo_nuevo;
	}else {
		cola->ultimo->prox= nodo_nuevo;
	}
	cola->ultimo= nodo_nuevo;
	return true;
}

void* cola_ver_primero(const cola_t *cola){
	if (cola_esta_vacia(cola)) return NULL;
	return cola->primero->dato;
}

void* cola_desencolar(cola_t *cola){
	if (cola_esta_vacia (cola)) return NULL;
	void* desencolado = cola->primero->dato;
	nodo_t* nodo_proximo = cola->primero->prox;
	if(cola->primero->prox == NULL) cola->ultimo = NULL;
	free (cola ->primero);
	cola->primero = nodo_proximo;
	return desencolado;
}

void cola_destruir(cola_t *cola, void destruir_dato(void*)){
	while (!cola_esta_vacia(cola)){
		if (destruir_dato){
			destruir_dato(cola_desencolar(cola));
	 	}else{
			cola_desencolar(cola);
		}
	}
	free(cola);
}

