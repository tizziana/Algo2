
#include "heap.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define TAMANIO_INICIAL 2000
#define CONSTANTE_DE_REDIMENSION 2


//Estructuras

struct heap{
  void **arreglo;
  cmp_func_t cmp;
  size_t cantidad;
  size_t tamanio;
};

//Funciones Auxiliares

void swap(void** arreglo1 ,void** arreglo2){
  void* aux = *arreglo1;
  *arreglo1 = *arreglo2;
  *arreglo2 = aux;
}

void upheap (void * arreglo [], cmp_func_t cmp, size_t posicion){
  if(posicion == 0) return;	
  size_t padre = (posicion-1)/2;
  if (cmp (arreglo[padre],arreglo[posicion]) < 0 ){
    swap(arreglo + padre ,arreglo + posicion);
    upheap(arreglo,cmp , padre);
  }	
}

void downheap (size_t cantidad, void* arreglo[],   cmp_func_t cmp, size_t posicion) {
  if(posicion >= cantidad) return;
  size_t maximo = posicion;
  size_t hijo_izq = 2*posicion+1 ;
  size_t hijo_der = 2*posicion +2 ;

  if(hijo_izq < cantidad && (cmp(arreglo[hijo_izq], arreglo[maximo]) > 0)) maximo = hijo_izq;

  if(hijo_der < cantidad && (cmp(arreglo[hijo_der], arreglo[maximo]) > 0)) maximo = hijo_der;

  if (maximo != posicion){
    swap(arreglo + maximo, arreglo + posicion);
    downheap(cantidad, arreglo, cmp, maximo);
  }
  return;
}

bool redimensionar(heap_t* heap, size_t tam_nuevo) {
  void** arreglo_nuevo = realloc(heap->arreglo, tam_nuevo*sizeof(void*));
  if(tam_nuevo > 0 && arreglo_nuevo == NULL) return false;

  heap->arreglo = arreglo_nuevo;
  heap->tamanio = tam_nuevo;
  return true;
}

void heapify (void* elementos[], size_t cantidad, cmp_func_t cmp){
  if (cantidad == 0 || !elementos) return;
  for (size_t i = cantidad ; i!= 0 ;i--){
    downheap(cantidad, elementos,cmp,i-1); //De las hojas a la raiz
  }
}

//Funciones del Heap
heap_t *heap_crear(cmp_func_t cmp) {
  heap_t *heap = malloc(sizeof(heap_t));
  if (!heap) return NULL;

  void** arreglo = malloc(TAMANIO_INICIAL * sizeof(void*));
  if (!arreglo) return NULL;
  heap->arreglo = arreglo;	
  heap->cmp = cmp;
  heap->cantidad = 0;
  heap->tamanio = TAMANIO_INICIAL;
  return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){	
  heap_t *heap = malloc(sizeof(heap_t));
  if (!heap) return NULL;

  void** arreglo_nuevo = malloc(sizeof(void*) * n);
  if (!arreglo_nuevo) return NULL;
  heap->arreglo = arreglo_nuevo;
  
  for (size_t i = 0; i < n; i++) {
        heap->arreglo[i] = arreglo[i];
  }
  heapify(heap->arreglo, n, cmp); //Lo convierto en un arreglo que cumple la condicion de heap

  heap->cmp = cmp;
  heap->cantidad = n;
  heap->tamanio = n;
  return heap;
}

void heap_destruir(heap_t* heap, void destruir_elemento(void *e)){
  for (int i= 0 ; i< heap_cantidad(heap); i++){
    if (destruir_elemento) destruir_elemento(heap->arreglo[i]);	
  }	
  free(heap->arreglo);
  free(heap);
}

size_t heap_cantidad(const heap_t *heap){
  return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap){
  if (heap->cantidad == 0) return true;
  return false;
}

bool heap_encolar(heap_t *heap, void *elem){
  if(heap->cantidad == heap->tamanio){
    if(!redimensionar(heap, (heap->tamanio * CONSTANTE_DE_REDIMENSION))){
      return false;
    }
  }

  heap->arreglo[heap->cantidad] = elem; //Agrego un elemento a la posicion final
  upheap(heap->arreglo,heap->cmp ,heap->cantidad);//UPHEAP para este elemento hasta cumplir la condicion de heap
  heap->cantidad ++;
  return true;
}

void *heap_ver_max(const heap_t *heap){
  if (heap_esta_vacio (heap)) return NULL;
  return heap->arreglo[0];
}

void *heap_desencolar(heap_t *heap){
  if (heap_esta_vacio (heap)) return NULL;
  void* eliminado = heap->arreglo [0]; //Guardo una referencia del que voy a eliminar
  heap->arreglo[0] = heap->arreglo[heap->cantidad-1]; //Considero al ultimo como primero/raiz
  downheap(heap->cantidad, heap->arreglo, heap->cmp, 0);//DOWNHEAP para este elemento hasta cumplir la condicion de heap
  heap->cantidad--;
  if (heap->cantidad == heap->tamanio / 4 && heap->tamanio != TAMANIO_INICIAL) {
    redimensionar(heap, heap->tamanio * CONSTANTE_DE_REDIMENSION);
  }
  return eliminado;
}

//Heapsort

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
  heapify(elementos, cant, cmp);	
  for (size_t i = cant ; i != 0 ; i--){
    swap(elementos, elementos + (i-1));
    downheap(i-1, elementos, cmp, 0);
  }
}
