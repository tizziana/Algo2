
#define _POSIX_C_SOURCE 200809L
#include "pila.h"
#include "abb.h"
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
 
 
typedef struct abb_nodo{
  void* dato;
  char* clave;
  struct abb_nodo* izquierdo;
  struct abb_nodo* derecho;
}abb_nodo_t;
 
struct abb{
  abb_nodo_t* raiz;
  abb_comparar_clave_t cmp;
  abb_destruir_dato_t destruir_dato;
  size_t cantidad;
};
 
//Auxiliar de abb
 
abb_nodo_t* buscar_padre (abb_t* arbol ,abb_nodo_t* nodo, const char* clave){
  if (arbol->cmp(clave, arbol->raiz->clave) == 0 || !nodo) return NULL;
  if (arbol->cmp(clave, nodo->clave) < 0){
    if(arbol->cmp(clave,nodo->izquierdo->clave) == 0) return nodo;
      return buscar_padre(arbol, nodo->izquierdo,clave);
  }
  else if (arbol->cmp(clave, nodo->clave) > 0){
    if(arbol->cmp(clave,nodo->derecho->clave) == 0) return nodo;
    return buscar_padre(arbol, nodo->derecho,clave);
  }
  return nodo;
}
 
abb_nodo_t* buscar_nodo(const abb_t* arbol, abb_nodo_t* nodo, const char* clave){
  if (!nodo) return NULL;
  if (arbol->cmp(clave, nodo->clave) < 0){
    return buscar_nodo(arbol,nodo->izquierdo,clave);
  }
  else if (arbol->cmp(clave, nodo->clave) > 0){
    return buscar_nodo(arbol, nodo->derecho,clave);
  }
  return nodo;
}

 

abb_nodo_t* _abb_guardar_rec(abb_t* arbol,abb_nodo_t* nodo ,const char* clave){
  if (arbol->cmp(clave, nodo->clave) < 0){
    if (!nodo->izquierdo) return nodo;
    return _abb_guardar_rec(arbol, nodo->izquierdo,clave);
    }
  else if (arbol->cmp(clave, nodo->clave) > 0){
    if (!nodo->derecho) return nodo;
    return _abb_guardar_rec(arbol, nodo->derecho,clave);
  }
  return nodo;
}

void destruir_nodo(abb_t* arbol, abb_nodo_t* nodo){
  if (arbol->destruir_dato) arbol->destruir_dato(nodo->dato);
  free(nodo->clave);
  free(nodo);
}

 
void desreferenciar_nodo (abb_nodo_t* padre, abb_nodo_t* nodo,abb_nodo_t* hijo, abb_t* arbol){
  if (padre->derecho == nodo) padre->derecho= hijo;
  else padre->izquierdo = hijo;
}

abb_nodo_t* recorrer_izquierda(abb_nodo_t* nodo){
  if (!nodo) return NULL;
  while (nodo->izquierdo) nodo= nodo->izquierdo;
  return nodo;
}

void _abb_destruir_rec(abb_t* arbol, abb_nodo_t* nodo){
  if (!nodo) return;
  _abb_destruir_rec(arbol, nodo->izquierdo);
  _abb_destruir_rec(arbol, nodo->derecho);
  destruir_nodo(arbol, nodo);

}




//Funciones del ABB
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
  abb_t* arbol = calloc (1, sizeof(abb_t));
  if (!arbol) return NULL;
  arbol->raiz = NULL;
  arbol->cmp = cmp;
  arbol->destruir_dato = destruir_dato;
  arbol->cantidad = 0;
  return arbol;
}
 
bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
  
 // Si la clave ya pertenece la modifico
  if (abb_pertenece(arbol,clave)){
    abb_nodo_t* nodo = buscar_nodo (arbol,arbol->raiz,clave);
    if (arbol->destruir_dato) arbol->destruir_dato(nodo->dato);
    nodo->dato = dato;
    return true;
  }
 //Si no pertenece, creo un nodo y lo guardo en el arbol
  abb_nodo_t* nodo_nuevo = calloc(1, sizeof(abb_nodo_t));
  if (!nodo_nuevo) return false;
  char* clave_nueva = calloc ((strlen(clave)+1), sizeof(char));
  if (!clave_nueva){
      free(nodo_nuevo);
      return false;
  }
  strcpy(clave_nueva, clave);
  nodo_nuevo->clave = clave_nueva;
  nodo_nuevo->dato = dato;
  nodo_nuevo->izquierdo = NULL;
  nodo_nuevo->derecho = NULL;
  arbol->cantidad ++;
  if(!arbol->raiz) {
    arbol->raiz = nodo_nuevo;
    return true;
  }
  abb_nodo_t* padre = _abb_guardar_rec(arbol, arbol->raiz,clave);
  if (arbol->cmp(clave, padre->clave) < 0) padre->izquierdo = nodo_nuevo;
  else if (arbol->cmp(clave, padre->clave) > 0) padre->derecho = nodo_nuevo;
  return true;
}
 
bool abb_pertenece(const abb_t *arbol, const char *clave){
  abb_nodo_t* nodo = buscar_nodo (arbol, arbol->raiz, clave);
  return nodo;
}
 
void *abb_borrar(abb_t *arbol, const char *clave){
  abb_nodo_t* nodo_a_borrar = buscar_nodo (arbol, arbol->raiz, clave);
  if(!nodo_a_borrar) return NULL;
  abb_nodo_t * padre = buscar_padre(arbol,arbol->raiz, clave);
  void *dato = nodo_a_borrar->dato;
  
  
  if (nodo_a_borrar->izquierdo && nodo_a_borrar->derecho) {
    abb_nodo_t * nodo_reemplazante = recorrer_izquierda(nodo_a_borrar->derecho);
    char* clave_reemplazante = strdup (nodo_reemplazante->clave);
    void* valor_reemplazante = abb_borrar(arbol, nodo_reemplazante->clave);
    free(nodo_a_borrar->clave);
    nodo_a_borrar->dato = valor_reemplazante; 
    nodo_a_borrar->clave = clave_reemplazante;
    return dato;
  }

  if (!padre) {
    if (nodo_a_borrar->izquierdo) arbol->raiz = nodo_a_borrar->izquierdo;
    else if (nodo_a_borrar->derecho) arbol->raiz = nodo_a_borrar->derecho;
    else arbol->raiz = NULL;
  }

  else if (nodo_a_borrar->izquierdo && !nodo_a_borrar->derecho) desreferenciar_nodo(padre, nodo_a_borrar, nodo_a_borrar->izquierdo, arbol);
  else if (!nodo_a_borrar->izquierdo && nodo_a_borrar->derecho) desreferenciar_nodo(padre, nodo_a_borrar, nodo_a_borrar->derecho, arbol);
  else if (!nodo_a_borrar->izquierdo && !nodo_a_borrar->derecho) desreferenciar_nodo(padre, nodo_a_borrar, NULL , arbol);
  destruir_nodo(arbol, nodo_a_borrar);
  arbol->cantidad --;
  return dato;
}
 
void *abb_obtener(const abb_t *arbol, const char *clave){
  abb_nodo_t* nodo = buscar_nodo (arbol, arbol->raiz, clave);
  if (!nodo) return NULL;
  return nodo->dato;
}
 
size_t abb_cantidad(abb_t *arbol){
  if (!arbol) return 0;
  return arbol->cantidad;
}
 
void abb_destruir(abb_t *arbol){
  _abb_destruir_rec(arbol,arbol->raiz);
  free(arbol);
}
 

//Funcion auxiliar del iterador interno}

bool rec_abb_in_order(abb_nodo_t *nodo, abb_comparar_clave_t cmp, const char * inicio, const char * final, bool visitar(const char *, void *, void *), void *extra){
    if (!nodo) return true;

    if (cmp(inicio, nodo->clave) > 0) return rec_abb_in_order(nodo->derecho, cmp, inicio, final, visitar, extra);
    if (cmp(nodo->clave, final) > 0) return rec_abb_in_order(nodo->izquierdo, cmp, inicio, final, visitar, extra);
    if (!rec_abb_in_order(nodo->izquierdo, cmp, inicio, final, visitar, extra) || !visitar(nodo->clave, nodo->dato, extra))
        return false;
    return rec_abb_in_order(nodo->derecho, cmp, inicio, final, visitar, extra);

}

void abb_in_order(abb_t *arbol, const char *desde, const char *hasta, bool visitar(const char *, void *, void *), void *extra) {
    rec_abb_in_order(arbol->raiz, arbol->cmp, desde, hasta, visitar, extra);
}
//Funciones de Iterador externo
struct abb_iter{
  pila_t* pila;
 
};
  
abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
  abb_iter_t* iterador = calloc (1,sizeof (abb_iter_t));
  if (!iterador) return NULL;
  pila_t* pila = pila_crear();
  if (!pila) return NULL;
  abb_nodo_t* actual = arbol->raiz;
  while (actual){
    pila_apilar(pila, actual);
    actual = actual->izquierdo;
  }
  iterador->pila= pila;
  return iterador;
}
 
bool abb_iter_in_avanzar(abb_iter_t *iter){
  if (abb_iter_in_al_final(iter)) return false;
  abb_nodo_t* desapilado = pila_desapilar(iter->pila);
  if (desapilado->derecho){
    abb_nodo_t* actual = desapilado->derecho;
    while (actual){
      pila_apilar(iter->pila, actual);
      actual = actual->izquierdo;
    }
  }
  return true;
}
 
const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
  if (pila_esta_vacia(iter->pila)) return NULL;
  abb_nodo_t* tope = pila_ver_tope (iter->pila);
  if (tope)return tope->clave;
  return NULL;
}
 
bool abb_iter_in_al_final(const abb_iter_t *iter){
  return (pila_esta_vacia(iter->pila));
}
 
void abb_iter_in_destruir(abb_iter_t* iter){
  pila_destruir(iter->pila);
  free (iter);
}



