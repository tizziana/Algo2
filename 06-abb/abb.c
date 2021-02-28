
/* *****************************************************************
* Entrega: Arbol de busqueda binaria
* Grupo: G20 
* Micaela Juarez (102784) / Tizziana Mazza Reta(101720)
* Corrector: Martin Buchwald
 * *****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "abb.h"
#include "pila.h"

/* *****************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct abb_nodo {
	void* dato;
	const char* clave;
	struct abb_nodo* izq;
	struct abb_nodo* der;
} abb_nodo_t;

struct abb {
	abb_nodo_t* raiz;
	abb_comparar_clave_t cmp;
	abb_destruir_dato_t destruir_dato;
	size_t cantidad;
};

struct abb_iter{
	const abb_t *arbol;
	pila_t *pila;
};

/* *****************************************************************
 *                 FUNCIONES AUXILIARES DE NODO
 * *****************************************************************/

char* strdup(const char* org) {
	size_t tamanio = strlen(org);
	char* dup = malloc(sizeof(char) * (tamanio + 1));
	
	if (!dup) {
		return NULL;
	}
	strcpy(dup, org);
	return dup;
}

abb_nodo_t* abb_nodo_crear(const char* clave, void* dato) {
	abb_nodo_t* nodo = malloc(sizeof(abb_nodo_t));
	if (!nodo) return NULL;
	nodo->clave = strdup(clave);
	
	if (!nodo->clave) {
		free(nodo);
		return NULL;
	}

	nodo->dato = dato;
	nodo->izq = NULL;
	nodo->der = NULL;
	return nodo;
}

void abb_nodo_destruir(abb_t* arbol, abb_nodo_t* nodo) {
	free((char*)nodo->clave);
	free(nodo);

}

/* *****************************************************************
 *                 FUNCIONES AUXILIARES DE ABB
 * *****************************************************************/

abb_nodo_t* abb_buscar_padre(const abb_t* arbol, abb_nodo_t* padre, const char* clave) {
	if (!padre) return NULL;
	int comparacion = (arbol->cmp)(padre->clave, clave);
	if (!comparacion) return padre;

	if (comparacion > 0) {
		if (!padre->izq) {
			return padre;
		}

		if (!(arbol->cmp)(padre->izq->clave, clave)) return padre;
		return abb_buscar_padre(arbol, padre->izq, clave);
	}

	if (!padre->der) {
		return padre;
	}

	if (!(arbol->cmp)(padre->der->clave, clave)) return padre;
	return abb_buscar_padre(arbol, padre->der, clave);
}

abb_nodo_t** abb_buscar(const abb_t* arbol, abb_nodo_t* nodo, const char* clave) {
	abb_nodo_t* padre = abb_buscar_padre(arbol, nodo, clave);
	if (!padre) return NULL;
	abb_nodo_t** padre_hijo = malloc(sizeof(abb_nodo_t) * 2);
	if (!padre_hijo) return NULL;
	padre_hijo[0] = padre;
	
	if (!(arbol->cmp)(padre->clave, clave)) padre_hijo[1] = padre;
	else if (padre->izq && (!(arbol->cmp)(padre->izq->clave, clave))) padre_hijo[1] = padre->izq;
	else if (padre->der && (!(arbol->cmp)(padre->der->clave, clave))) padre_hijo[1] = padre->der;
	else padre_hijo[1] = NULL;

	return padre_hijo;
}

/* *****************************************************************
 *                 	   PRIMITIVAS DEL ABB
 * *****************************************************************/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
	abb_t* arbol = malloc(sizeof(abb_t));
	if (!arbol) return NULL;

	arbol->cantidad = 0;
	arbol->raiz = NULL;
	arbol->cmp = cmp;
	arbol->destruir_dato = destruir_dato;
	return arbol;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
	if (!arbol || !clave) return false;

	abb_nodo_t* padre = NULL;
	abb_nodo_t* nodo = NULL;

	abb_nodo_t** padre_hijo = abb_buscar(arbol, arbol->raiz, clave);

	if (padre_hijo) {
		padre = padre_hijo[0];
		nodo = padre_hijo[1];
	}

	if (nodo) {
		if (arbol->destruir_dato) (arbol->destruir_dato)(nodo->dato);
		nodo->dato = dato;
		free(padre_hijo);
		return true;
	}

	nodo = abb_nodo_crear(clave, dato);
	
	if (!nodo) {
		free(padre_hijo);
		return false;
	}
	
	if (!arbol->raiz) arbol->raiz = nodo;
	else {
		if ((arbol->cmp)(padre->clave, clave) > 0) padre->izq = nodo;
		else padre->der = nodo;
	}

	free(padre_hijo);
	arbol->cantidad++;
	return true;
}

void* abb_borrar(abb_t* arbol, const char* clave) {
	if (!clave || !arbol->raiz) return NULL;

	abb_nodo_t** padre_hijo = abb_buscar(arbol, arbol->raiz, clave);
	if (!padre_hijo) return NULL;
	
	if (!padre_hijo[1]) {
		free(padre_hijo);
		return NULL;
	}

	abb_nodo_t* padre = padre_hijo[0];
	abb_nodo_t* nodo = padre_hijo[1];
	void* dato = nodo->dato;
	bool es_raiz = false;
	if (!(arbol->cmp)(padre->clave, nodo->clave)) es_raiz = true;

	if (!nodo->der || !nodo->izq) {
		if (nodo->der) { 
			if (es_raiz) arbol->raiz = nodo->der;
			else {
				if ((arbol->cmp)(padre->clave, clave) > 0) padre->izq = nodo->der;
				else padre->der = nodo->der;
			}
		}
		else {
			if (es_raiz) arbol->raiz = nodo->izq;	
			else {
				if ((arbol->cmp)(padre->clave, clave) > 0) padre->izq = nodo->izq;
				else padre->der = nodo->izq;
			}
		}
		
		abb_nodo_destruir(arbol, nodo);
		arbol->cantidad--;
		
	} else {
		abb_nodo_t* reemplazante = nodo->der;

		while (reemplazante->izq) {
			reemplazante = reemplazante->izq;	
		}
		char* clave_reemplazante = strdup(reemplazante->clave);
		nodo->dato = abb_borrar(arbol, reemplazante->clave);
		free((char*)nodo->clave);
		nodo->clave = clave_reemplazante;
	}

	free(padre_hijo);
	return dato;
}

void* abb_obtener(const abb_t* arbol, const char* clave) {
	if (!arbol || !clave) return NULL;
	abb_nodo_t** padre_hijo = abb_buscar(arbol, arbol->raiz, clave);
	if (!padre_hijo) return NULL;
	void* dato = NULL;
	if (padre_hijo[1]) dato = padre_hijo[1]->dato;
	free(padre_hijo);
	return dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
	if (!arbol || !clave) return false;
	abb_nodo_t** padre_hijo = abb_buscar(arbol, arbol->raiz, clave);
	if (!padre_hijo) return false;
	bool pertence = padre_hijo[1];
	free(padre_hijo);
	return pertence;
}

size_t abb_cantidad(abb_t *arbol) {
	return arbol->cantidad;
}

void _abb_destruir(abb_t* arbol, abb_nodo_t* nodo){
	if (!nodo) return;
	if (nodo->izq) _abb_destruir(arbol, nodo->izq);
	if (nodo->der) _abb_destruir(arbol, nodo->der);
	if (arbol->destruir_dato) (arbol->destruir_dato)(nodo->dato);
	abb_nodo_destruir(arbol, nodo);
}

void abb_destruir(abb_t *arbol){
	_abb_destruir(arbol, arbol->raiz);
	free(arbol);
}

/* *****************************************************************
 *           FUNCOINES AUXILIARES DEL ITERADOR EXTERNO
 * *****************************************************************/

void apilar_rama_izquierda(pila_t* pila, abb_nodo_t* nodo_actual) {
	while(nodo_actual) {
		pila_apilar(pila, nodo_actual);
		nodo_actual = nodo_actual->izq;
	}
}

/* *****************************************************************
 *                PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
	abb_iter_t* abb_iter = malloc(sizeof(abb_iter_t));
	if(!abb_iter) return NULL;

	abb_iter->pila = pila_crear();
	if(!abb_iter->pila) {
		free(abb_iter);
		return NULL;
	}

	apilar_rama_izquierda(abb_iter->pila, arbol->raiz);
	abb_iter->arbol = arbol;
	return abb_iter;
}

bool abb_iter_in_al_final(const abb_iter_t *iter) {
	return (pila_esta_vacia(iter->pila));
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
	if (abb_iter_in_al_final(iter)) return false;

	abb_nodo_t* nodo_actual = pila_desapilar(iter->pila);
	if (nodo_actual->der) { 
		pila_apilar(iter->pila, nodo_actual->der);
		nodo_actual = nodo_actual->der->izq;
	
		apilar_rama_izquierda(iter->pila, nodo_actual);
	}
	return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {
	if (pila_esta_vacia(iter->pila)) return NULL;
	return ((abb_nodo_t*)pila_ver_tope(iter->pila))->clave;
}

void abb_iter_in_destruir(abb_iter_t* iter) {
	pila_destruir(iter->pila);
	free(iter);
}

/* *****************************************************************
 *                PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/

bool _abb_in_order(abb_nodo_t* nodo, bool visitar(const char *, void *, void *), void *extra) {
	if (!nodo) return true;

	bool estado = _abb_in_order(nodo->izq, visitar, extra);
	if (estado) estado = visitar(nodo->clave, nodo->dato, extra);
	if (estado) estado = _abb_in_order(nodo->der, visitar, extra);
	return estado;
}

void abb_in_order(abb_t *arbol, bool funcion(const char *, void *, void *), void *extra){
	if (!arbol || !arbol->raiz) return;
	_abb_in_order(arbol->raiz, funcion, extra);
}