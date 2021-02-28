#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

typedef struct nodo{
	struct nodo* prox;
	void* dato;
}nodo_t;

typedef struct lista{
	nodo_t* prim;
	nodo_t* ult;
	size_t largo;	

}lista_t;


typedef struct lista_iter{
	nodo_t* actual;
	nodo_t* anterior;
	lista_t* lista;
} lista_iter_t;

// Primitivas de la Lista Enlazada

lista_t *lista_crear(void) {
	lista_t *lista = malloc(sizeof(lista_t));
	if (!lista) return NULL;
	lista->prim = NULL;
	lista->ult = NULL;
	lista->largo = 0;
	return lista;
}

nodo_t* nodo_crear (void*dato){
	nodo_t* nodo= malloc(sizeof(nodo_t));
	if (!nodo) return NULL;
	nodo->dato = dato;
	nodo->prox = NULL;
	return nodo;
}


bool lista_esta_vacia(const lista_t *lista) {
	return lista->largo == 0;
}

bool lista_insertar_primero(lista_t *lista, void *dato) {
	nodo_t *nodo_nuevo = nodo_crear (dato);
	if (!nodo_nuevo) return false;
	if (lista_esta_vacia(lista)) lista->ult = nodo_nuevo;
	nodo_nuevo->prox = lista->prim;
	lista->prim = nodo_nuevo;
	lista->largo ++;
	return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato) {
	nodo_t *nodo_nuevo = nodo_crear (dato);
	if (!nodo_nuevo) return false;
	nodo_nuevo->prox = NULL;
	if (lista_esta_vacia(lista)) lista->prim = nodo_nuevo;
	else lista->ult->prox = nodo_nuevo;
	lista->ult = nodo_nuevo;
	lista->largo ++;
	return true;
}
void *lista_borrar_primero(lista_t *lista) {
	if (lista_esta_vacia(lista)) return NULL;
	void *dato_borrado = lista->prim->dato;
	nodo_t *primer_nodo = lista->prim;
	lista->prim = lista->prim->prox;
	free(primer_nodo);
	lista->largo --;
	if (lista->largo == 0) lista->ult = NULL;
	return dato_borrado;
}
void *lista_ver_primero(const lista_t *lista) {
	if (lista_esta_vacia(lista)) return NULL;
	return lista->prim->dato;
}
void *lista_ver_ultimo(const lista_t* lista) {
	if (lista_esta_vacia(lista)) return NULL;
	return lista->ult->dato;
}
size_t lista_largo(const lista_t *lista) {
	return lista->largo;
}
void lista_destruir(lista_t *lista, void destruir_dato(void *)) {
	while (!lista_esta_vacia(lista)) {
		if (destruir_dato) {
			destruir_dato(lista_borrar_primero(lista));
		} else {
			lista_borrar_primero(lista);
		}
	}
	free(lista);
}


//Primitiva del iterador interno

void lista_iterar(lista_t *lista, bool visitar(void*, void*), void *extra) {
	nodo_t *actual = lista->prim;
	while (actual) {
		if (visitar) {
			if (!visitar(actual->dato, extra)) break;
		}
		actual = actual->prox;
	}
}

// Primitivas del iterador externo.

lista_iter_t *lista_iter_crear(lista_t *lista) {
	lista_iter_t *iterador = malloc(sizeof(lista_iter_t));
	if (!iterador) return NULL;
	iterador->actual = lista->prim;
	iterador->anterior = NULL;
	iterador->lista = lista;
	return iterador;
}

bool lista_iter_al_final(const lista_iter_t *iter) {
	return iter->actual == NULL;

}

bool lista_iter_avanzar(lista_iter_t *iter) {
	if (lista_esta_vacia(iter->lista)) return false;
	if (lista_iter_al_final(iter)) return false; 
	iter->anterior = iter->actual;
	iter->actual = iter->actual->prox;
	return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter) {
	if (!iter->actual) return NULL;
	return iter->actual->dato;
}
void lista_iter_destruir(lista_iter_t *iter) {
	free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato) {
	nodo_t *nuevo_nodo = nodo_crear (dato);
	
	if (!nuevo_nodo) return false;
	nuevo_nodo->prox = iter->actual;
	iter->actual = nuevo_nodo;	
	if (lista_esta_vacia (iter->lista)){
		iter->lista->prim = nuevo_nodo;
		iter->lista->ult = nuevo_nodo;
	}
	else if (!iter->anterior) iter->lista->prim = nuevo_nodo; 
	else iter->anterior->prox = nuevo_nodo;
	if (iter->anterior == iter->lista->ult) iter->lista->ult = nuevo_nodo;
	iter->lista->largo ++;	
	return true;
}

void *lista_iter_borrar(lista_iter_t *iter) {
	if (!iter->actual) return NULL;
	void *dato_a_borrar = iter->actual->dato;
	nodo_t *siguiente = iter->actual->prox;
	if (!iter->anterior) iter->lista->prim = siguiente;
	else iter->anterior->prox= siguiente;	
	if (iter->actual == iter->lista->ult) iter->lista->ult = iter->anterior;	
	free(iter->actual);
	iter->actual = siguiente;
	iter->lista->largo --;	
	return dato_a_borrar;
}






