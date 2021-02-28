#include "hash.h"
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"


#define INITIAL_VALUE 15381
#define M 33 //Kernighan and Ritchie's function
#define TAMANIO_INICIAL 20
#define LIMITE_SUP 2
#define LIMITE_INF 1
#define REDIM_SUP 2
#define REDIM_INF 2

struct hash{
    size_t cantidad;
    size_t tamanio;
    lista_t ** tabla;
    hash_destruir_dato_t destruir_dato;
};

typedef struct hash_campo{
    char* clave;
    void* valor;
}hash_campo_t;

struct hash_iter{
    size_t posicion;
    const hash_t* hash;
    lista_iter_t* iter;
    size_t cant_iterados;
};

//Funcion de Hashing

 size_t hashing(const char *key, size_t capacidad) {
   size_t hash = INITIAL_VALUE;
   for(size_t i = 0; i < strlen(key); ++i) hash = M * hash + key[i];
   return hash % capacidad;
}

// Funciones Auxiliares

lista_t** crear_lista(hash_t* hash, size_t tamanio){
   lista_t** tabla = malloc(tamanio* sizeof(lista_t*));
   if (!tabla) return NULL;
   for (size_t i = 0; i < tamanio; i++) {
      tabla[i] = lista_crear();
      if (!tabla[i]) {
         for (size_t j = 0; j < i; j++) {
            lista_destruir(tabla[i], NULL);
         }
         hash_destruir(hash);
         return NULL;
      }
   }
   return tabla;
}

bool hash_redimensionar(hash_t* hash, size_t tamanio_nuevo) {
   lista_t** tabla_nueva = crear_lista(hash, tamanio_nuevo);
   for (int i = 0; i < hash->tamanio; i++) {
      if (lista_esta_vacia(hash->tabla[i])) {
         lista_destruir(hash->tabla[i], NULL);
         continue;
      }
      lista_iter_t* iter = lista_iter_crear(hash->tabla[i]);
      if (!iter) return false;
      while(!lista_iter_al_final(iter)) {
         hash_campo_t* campo = lista_iter_borrar(iter);
         size_t indice = hashing(campo->clave, tamanio_nuevo);
         lista_insertar_ultimo(tabla_nueva[indice], campo);
      }
      lista_iter_destruir(iter);
      lista_destruir(hash->tabla[i], NULL);
   }
   free(hash->tabla);
   hash->tabla = tabla_nueva;
   hash->tamanio = tamanio_nuevo;
   return true;
}

void destruir_campo (hash_campo_t* campo, hash_destruir_dato_t destruir_dato){
   if (destruir_dato) destruir_dato(campo->valor);
   free(campo->clave);
   free(campo);
}

lista_iter_t* buscar_iter(const hash_t *hash, const char* clave){

   size_t indice = hashing(clave,hash->tamanio);
   lista_iter_t* iter = lista_iter_crear(hash->tabla[indice]);
   if(!iter) return NULL;
   for(; !lista_iter_al_final(iter);lista_iter_avanzar(iter) ){
      hash_campo_t* actual = lista_iter_ver_actual(iter);
      if(strcmp(actual->clave, clave) == 0) return iter;
   }
   lista_iter_destruir(iter);
   return NULL;
}

//Funciones del hash

hash_t* hash_crear(hash_destruir_dato_t destruir_dato){
   hash_t* hash = malloc(sizeof(hash_t));
   if(!hash) return NULL;
   hash->cantidad = 0;
   hash->tamanio = TAMANIO_INICIAL;
   lista_t** tabla = crear_lista(hash, TAMANIO_INICIAL);
   if (!tabla) return NULL;
   hash->tabla = tabla;
   hash->destruir_dato = destruir_dato;
   return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void* dato){
   if(hash->cantidad / hash->tamanio > LIMITE_SUP){
      if (!hash_redimensionar(hash, hash->tamanio * REDIM_SUP)) return false;
   }
   //si pertenece tengo que cambiar el dato

   if(hash_pertenece(hash, clave)){
      lista_iter_t* iter = buscar_iter(hash, clave); 
      hash_campo_t* campo = lista_iter_ver_actual(iter);
      if (hash->destruir_dato) {
         hash->destruir_dato(campo->valor);
      }
      campo->valor = dato;
      lista_iter_destruir(iter);   
      return true;
   }

   size_t indice = hashing(clave,hash->tamanio);
   //pido memoria para un campo
   hash_campo_t* nodo_campo = malloc(sizeof(hash_campo_t));
   if (!nodo_campo) return NULL;
   // pido memoria para la clave
   char* clave_nueva = calloc((strlen(clave)+1),sizeof(char));
   strcpy(clave_nueva, clave);
   nodo_campo->valor = dato;
   nodo_campo->clave = clave_nueva;
   if (!lista_insertar_ultimo(hash->tabla[indice], nodo_campo))return false;
   hash->cantidad ++;
   return true;

}

void* hash_borrar(hash_t * hash, const char * clave){

   if(hash_cantidad(hash) == 0) return NULL;

   lista_iter_t* iter = buscar_iter(hash, clave);
   if (!iter) return NULL;
   hash_campo_t* actual = lista_iter_borrar(iter);
   void* valor = actual->valor;
   destruir_campo (actual, NULL);
   hash->cantidad --;
   lista_iter_destruir(iter);
   if(hash->cantidad / hash-> tamanio < LIMITE_INF && hash->tamanio > TAMANIO_INICIAL){
      if (!hash_redimensionar(hash, hash->tamanio / REDIM_INF)) return NULL;
   }
   return valor;
}

size_t hash_cantidad(const hash_t * hash){
   return hash->cantidad;
}

void* hash_obtener(const hash_t *hash, const char* clave){
   lista_iter_t* iter = buscar_iter( hash, clave);
   if (!iter) return NULL;
   hash_campo_t* campo = lista_iter_ver_actual(iter);
   lista_iter_destruir(iter);
   return campo->valor;
}

void hash_destruir(hash_t* hash){
   for(int i=0 ;i< hash->tamanio; i++){
      while(!lista_esta_vacia(hash->tabla[i])){
         hash_campo_t *aux = lista_borrar_primero(hash->tabla[i]);
         destruir_campo(aux, hash->destruir_dato);
      }
      lista_destruir(hash->tabla[i],NULL);
   }
   free(hash->tabla);
   free(hash);
}


bool hash_pertenece (const hash_t* hash, const char * clave){

   bool pertenece = false;
   lista_iter_t* iter = buscar_iter( hash, clave);
   if(iter) pertenece = true;
   lista_iter_destruir(iter);
   return pertenece;
}


//Funciones Auxiliares del iterador
size_t buscar_proximo_indice(hash_iter_t *iter){
   for (size_t i=iter->posicion+1; i<iter->hash->tamanio ;i++){
      if (!lista_esta_vacia(iter->hash->tabla[i]))return i;
   }
   return 0;
}

//Funciones del Iterador
hash_iter_t *hash_iter_crear(const hash_t *hash){

   hash_iter_t *iterador = malloc(sizeof(hash_iter_t));
   if (!iterador) return NULL;
   iterador->posicion = 0;
   iterador->hash = hash;
   while (iterador->posicion < hash->tamanio -1 && lista_esta_vacia(hash->tabla[iterador->posicion])){
      iterador->posicion++;
   }
   iterador->cant_iterados = 0;
   iterador->iter = lista_iter_crear(hash->tabla[iterador->posicion]);
   if(!iterador->iter) return NULL;
   return iterador;
}

bool hash_iter_avanzar(hash_iter_t *iter){

   if (hash_iter_al_final(iter)) return false; 
 //si no esta al final avanza en la lista que se encuentra
   
   lista_iter_avanzar(iter->iter);
   if (!lista_iter_al_final(iter->iter)) { 
 //si esta al final de una lista, buscara la siguiente lista
      iter->cant_iterados++;
      return true;
   }
   lista_iter_destruir(iter->iter);
   size_t indice = buscar_proximo_indice(iter);
   if(indice == 0){
      iter->posicion = iter->hash->tamanio;
      iter->cant_iterados = iter->hash->cantidad;
      return true;
   }

 //creo un iterador en la lista indicada
   iter->posicion = indice;
   iter->iter = lista_iter_crear(iter->hash->tabla[indice]);
   iter->cant_iterados++;
   return true;

}


bool hash_iter_al_final(const hash_iter_t * iter){
   return (iter->cant_iterados == hash_cantidad(iter->hash));
}

char *hash_iter_ver_actual(const hash_iter_t *iter){
   if(hash_iter_al_final(iter)) return NULL;
   hash_campo_t* campo_nuevo = lista_iter_ver_actual(iter->iter);
   if (!campo_nuevo) return NULL;
   return campo_nuevo->clave;
}

void hash_iter_destruir(hash_iter_t* iter){
   if(iter->hash->cantidad == 0) free(iter->iter);
   free(iter);
}
