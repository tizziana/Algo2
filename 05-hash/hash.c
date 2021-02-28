
/* *****************************************************************
* Entrega: Hash.
* Grupo: G20.
* Micaela Juarez (102784) / Tizziana Mazza Reta(101715).
* Corrector: Martin Buchwald.
 * *****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "lista.h"
#define CONSTANTE_HASHING_1 16777619
#define CONSTANTE_HASHING_2 2166136261
#define VOLUMEN_INICIAL 200
#define FACTOR_CARGA 2
#define REDUCIR 4
#define	REDIMENSION 2


/* *****************************************************************
 *                    DEFINICIÓN DE ESTRUCTURAS
 * *****************************************************************/

typedef void (*hash_destruir_dato_t)(void *);

typedef struct hash {
	lista_t** tabla;
	size_t tam;
	size_t ocupado;
	hash_destruir_dato_t destruir_dato;
}hash_t;

typedef struct campo_hash {
	char* key;
	void* dato;
} campo_hash_t;

typedef struct iter_hash {
	size_t pos;
	const hash_t* hash;
	lista_iter_t* lista_iter;
}hash_iter_t;


/* *****************************************************************
 *                 	    FUNCIONES AUXILIARES
 * *****************************************************************/

/* Original: http://ctips.pbworks.com/w/page/7277591/FNV%20Hash */
size_t funcion_hashing(const char* clave) {
    size_t hashing = CONSTANTE_HASHING_2;
    size_t largo = strlen(clave);

    for(size_t i = 0; i < largo; i++){
        hashing ^= (clave[i]);
        hashing *= CONSTANTE_HASHING_1;
    }

    return hashing;
}

char* duplicar(const char* org) {
	size_t longitud = strlen(org);
	char* dup = malloc(sizeof(char) * (longitud + 1));
	if (!dup) return NULL;
	strcpy(dup, org);
	return dup;
}

void rehash(hash_t* hash, lista_t** tabla_nueva, size_t tam_nuevo) {
	for (int i = 0; i < hash->tam; i++){
		lista_iter_t* iter = lista_iter_crear(hash->tabla[i]);
		
		while (!lista_iter_al_final(iter)){
			campo_hash_t* campo_hash = lista_iter_borrar(iter);
			lista_insertar_ultimo(tabla_nueva[funcion_hashing(campo_hash->key) % tam_nuevo], campo_hash);
		}

		lista_destruir(hash->tabla[i], NULL);
		lista_iter_destruir(iter);
	}
}

lista_t** hash_tabla_crear (hash_t* hash, size_t tamanio_nuevo, bool redimensionar) {
	lista_t** tabla = malloc(sizeof(lista_t*) * tamanio_nuevo);
	if (!tabla) { 
		free(hash);
		return NULL;
	}

	for(size_t i = 0; i < tamanio_nuevo; i++) { 
		tabla[i] = lista_crear();
		if (!tabla[i]) return NULL;
	}

	if (redimensionar) {
		rehash(hash, tabla, tamanio_nuevo);
		free (hash->tabla);
	}
	else hash->ocupado = 0;
	hash->tabla = tabla;
	hash->tam = tamanio_nuevo;
	
	return tabla;
}

bool hash_redimensionar(hash_t* hash, size_t tam_nuevo) {
	lista_t** tabla_nueva = hash_tabla_crear(hash, tam_nuevo, true);
	if (!tabla_nueva) return false;
	return true;
}

campo_hash_t* campo_hash_crear(const char* clave, void *dato) {
	campo_hash_t* campo_hash = malloc(sizeof(campo_hash_t));
	char * clave_dinamica = duplicar(clave);
	if (!campo_hash) return false;
	
	if (!clave_dinamica) { 
		free(campo_hash);
		return false;
	}
	
	campo_hash->key = clave_dinamica;
	campo_hash->dato = dato;

	return campo_hash;
}

lista_iter_t* hash_buscar(const hash_t* hash, const char* clave, size_t posicion) {
	lista_iter_t* iter = lista_iter_crear(hash->tabla[posicion]);

	while (!lista_iter_al_final(iter)) {
		if (!strcmp(((campo_hash_t*)lista_iter_ver_actual(iter))->key, clave)) {
			return iter;
		}

		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);
	return NULL;
}

/* *****************************************************************
 *                    PRIMITIVAS DEL HASH
 * *****************************************************************/

hash_t* hash_crear(hash_destruir_dato_t destruir_dato) {
	hash_t* hash = malloc(sizeof(hash_t));
	if (!hash) return NULL;

	lista_t** tabla = hash_tabla_crear(hash, VOLUMEN_INICIAL, false);
	if (!tabla) { 
		free(hash);
		return NULL;
	}
	hash->destruir_dato = destruir_dato;

	return hash;
}

bool hash_guardar(hash_t *hash, const char* clave, void *dato) {
	if (!clave) return false;
	
	lista_iter_t* iter;
	campo_hash_t* campo;

	size_t posicion = funcion_hashing(clave) % hash->tam;

	if ((iter = hash_buscar(hash, clave, posicion))) {
		campo = lista_iter_ver_actual(iter);
		if (hash->destruir_dato) (hash->destruir_dato)(campo->dato);
		campo->dato = dato;
		lista_iter_destruir(iter);
		return true;
	}

	if (hash->ocupado / hash->tam >= FACTOR_CARGA) {
		size_t tamanio_nuevo = hash->tam * REDIMENSION;
		if (!hash_redimensionar(hash, tamanio_nuevo)) return false;
	}

	campo = campo_hash_crear(clave, dato);

	if (!lista_insertar_ultimo(hash->tabla[posicion], campo)) {
		free(campo->key);
		free(campo);
		return false;
	}

	hash->ocupado++;
	return true;
}

void *hash_borrar(hash_t *hash, const char *clave) {
	if (!clave) return NULL;

	if ((hash->ocupado != 0) && (hash->ocupado / hash->tam <= FACTOR_CARGA / REDUCIR)) {
		size_t tamanio_nuevo = hash->tam / REDIMENSION;
		if (!hash_redimensionar(hash, tamanio_nuevo)) return NULL;
	}

	lista_iter_t* iter = hash_buscar(hash, clave, funcion_hashing(clave) % hash->tam);
	if (!iter) return NULL;
	campo_hash_t* campo_a_borrar = lista_iter_borrar(iter);
	void* dato = campo_a_borrar->dato;
	free(campo_a_borrar->key);
	free(campo_a_borrar);
	lista_iter_destruir(iter);
	hash->ocupado--;
	return dato;
}

void *hash_obtener(const hash_t *hash, const char *clave){
	if (!clave) return NULL;
	lista_iter_t* iter = hash_buscar(hash, clave, funcion_hashing(clave) % hash->tam);
	if (!iter) return NULL;
	void* dato = ((campo_hash_t*)(lista_iter_ver_actual(iter)))->dato;
	lista_iter_destruir(iter);
	return dato;
}


bool hash_pertenece(const hash_t *hash, const char *clave){
	if (!clave) return false;
	lista_iter_t* iter = hash_buscar(hash, clave, funcion_hashing(clave) % hash->tam);
	if (!iter) return false;
	lista_iter_destruir(iter);
	return true;
}

size_t hash_cantidad(const hash_t *hash) {
	return hash->ocupado;
}

void hash_destruir(hash_t *hash) {
	for (int i = 0; i < hash->tam; i++) {
		lista_iter_t* iter = lista_iter_crear(hash->tabla[i]);

		while (!lista_iter_al_final(iter)) {
			campo_hash_t* campo_a_borrar = lista_iter_borrar(iter);
			
			if (hash->destruir_dato) {
				(hash->destruir_dato)(campo_a_borrar->dato);
			}

			free(campo_a_borrar->key);
			free(campo_a_borrar);
		}

		lista_iter_destruir(iter);
		lista_destruir(hash->tabla[i], NULL);
	}

	free(hash->tabla);
	free(hash);
}

/* *****************************************************************
 *                    PRIMITIVAS DEL ITERADOR
 * *****************************************************************/

bool hash_iter_al_final(const hash_iter_t *iter) {
	if (!iter->hash->ocupado) return true;
	return iter->pos == iter->hash->tam;
}

lista_iter_t* hash_iter_avanzar_lista(hash_iter_t* iter) {
	lista_iter_t* lista_iter = lista_iter_crear(iter->hash->tabla[iter->pos]);

	while (!hash_iter_al_final(iter)){
		if (!lista_iter_al_final(lista_iter)) {
			return lista_iter;
		}
		
		lista_iter_destruir(lista_iter);
		iter->pos++;
		if (hash_iter_al_final(iter)) return NULL;
		lista_iter = lista_iter_crear(iter->hash->tabla[iter->pos]);
		if (!lista_iter) return NULL;
	}

	lista_iter_destruir(lista_iter);
	return NULL;
}

hash_iter_t *hash_iter_crear(const hash_t *hash) {
	hash_iter_t * iter = malloc(sizeof(hash_iter_t));
	if(!iter) return NULL;
	iter->pos = 0; 
	iter->hash = hash;
	iter->lista_iter = hash_iter_avanzar_lista(iter);
	return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter) {
	if (hash_iter_al_final(iter)) return false; 
	if (!lista_iter_al_final(iter->lista_iter)) {
		lista_iter_avanzar(iter->lista_iter);
		if (!lista_iter_al_final(iter->lista_iter)) return true;
	}

	lista_iter_destruir(iter->lista_iter);
	iter->pos++;
	if (hash_iter_al_final(iter)) return false;
	iter->lista_iter = hash_iter_avanzar_lista(iter);
	if (!iter->lista_iter) return false;

	return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter) {
	if (hash_iter_al_final(iter)) return NULL;
	return ((campo_hash_t*)lista_iter_ver_actual(iter->lista_iter))->key;
}

void hash_iter_destruir(hash_iter_t* iter) {
	if (!hash_iter_al_final(iter)) lista_iter_destruir(iter->lista_iter);
	free(iter);
}