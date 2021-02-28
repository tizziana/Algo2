
/* *****************************************************************
* Entrega: Heap.
* Grupo: G20.
* Micaela Juarez (102784) / Tizziana Mazza Reta(101715).
* Corrector: Martin Buchwald.
* *****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "heap.h"
#define TAM_INICIAL 200
#define AUMENTAR 2
#define REDUCIR 4

/* *****************************************************************
 *				DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef int (*cmp_func_t) (const void *a, const void *b);

 struct heap{
	void** vector;
	size_t tamanio;
	size_t cantidad;
	cmp_func_t cmp;
 };

/* *****************************************************************
 *						FUNCIONES AUXILIARES
 * *****************************************************************/

heap_t* heap_crear_aux(size_t tamanio_inicial,size_t cantidad_inicial, cmp_func_t cmp) {
	heap_t* heap = malloc(sizeof(heap_t));
	if (!heap) return NULL;

	void** vector = malloc(sizeof(void*) * tamanio_inicial);
	if (!vector) {
		free(heap);
		return NULL;
	}

	heap->vector = vector;
	heap->tamanio = tamanio_inicial;
	heap->cantidad = cantidad_inicial;
	heap->cmp = cmp;

	return heap;
}

 bool heap_redimensionar(heap_t* heap, size_t tam_nuevo) {
	void** vector_nuevo = realloc(heap->vector, tam_nuevo * sizeof(void*));
	if (vector_nuevo == NULL) {
		return false;
	}

	heap->vector = vector_nuevo;
	heap->tamanio = tam_nuevo;
	return true;
}

void heap_swap_elemento(void** vector, size_t posicion1, size_t posicion2) {
	void *aux = vector[posicion1];
	vector[posicion1] = vector[posicion2];
	vector[posicion2] = aux;
}

size_t buscar_padre(size_t posicion) {
	return (posicion - 1) / 2;
}

void upheap(void** arreglo, size_t pos, cmp_func_t cmp) {
	if (!pos) return;
	size_t pos_padre = buscar_padre(pos);
	if (cmp(arreglo[pos], arreglo[pos_padre]) < 0) return;
	heap_swap_elemento(arreglo, pos, pos_padre);
	upheap(arreglo, pos_padre, cmp);
}

size_t posicion_hijo_izq(void** arreglo, size_t posicion) {
	return (2 * posicion + 1);
}

void downheap(void** arreglo, size_t tam, size_t pos, cmp_func_t cmp) {
	if (pos >= tam) return;
	size_t pos_h_izq = posicion_hijo_izq(arreglo, pos);
	size_t pos_h_der = pos_h_izq + 1;
	size_t pos_max = pos;

	if ((pos_h_izq < tam) && (cmp(arreglo[pos_h_izq], arreglo[pos_max]) > 0)) {
		pos_max = pos_h_izq;
	}

	if ((pos_h_der < tam) && (cmp(arreglo[pos_h_der], arreglo[pos_max]) > 0)) {
		pos_max = pos_h_der;
	}

	if (pos != pos_max) {
		heap_swap_elemento(arreglo, pos, pos_max);
		downheap(arreglo, tam, pos_max, cmp);
	}
}

/* *****************************************************************
 *							HEAPSORT
 * *****************************************************************/

void heapify(void** arreglo, size_t cantidad, cmp_func_t cmp) {
	for (int i = (int)cantidad - 1; i >= 0; i--) {
		downheap(arreglo, cantidad, i, cmp);
	}
}

void heap_sort(void** elementos, size_t cant, cmp_func_t cmp) {
	heapify(elementos, cant, cmp);

	for (int i = (int)cant - 1; i >= 0; i--) {
		heap_swap_elemento(elementos, 0, i);
		downheap(elementos, i, 0, cmp);
	}
}

/* *****************************************************************
 *                 	   PRIMITIVAS DEL HEAP
 * *****************************************************************/

heap_t* heap_crear(cmp_func_t cmp) {
	heap_t* heap = heap_crear_aux(TAM_INICIAL, 0, cmp);
	if (!heap) return NULL;
	return heap;
}

heap_t* heap_crear_arr(void* arreglo[], size_t n, cmp_func_t cmp) {
	heap_t* heap = heap_crear_aux(n*2, n, cmp);
	if (!heap) return NULL;

	if (arreglo) {
		for (size_t i = 0; i < n; i++) {
			heap->vector[i] = arreglo[i];
		}
		heapify(heap->vector, n, cmp);
	}
	return heap;
}

size_t heap_cantidad(const heap_t *heap) {
	return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap) {
	return !heap_cantidad(heap);
}

bool heap_encolar(heap_t *heap, void *elem){
	if (!elem) return false;

	if (heap->cantidad == heap->tamanio) {
		if (!heap_redimensionar(heap, (heap->tamanio) * AUMENTAR)) return false;
	}

	heap->vector[heap->cantidad] = elem;
	upheap(heap->vector, heap->cantidad, heap->cmp);
	heap->cantidad++;
	return true;
}

void* heap_ver_max(const heap_t *heap){
	if (heap_esta_vacio(heap)) return NULL;
	return heap->vector[0];
}

void* heap_desencolar(heap_t *heap) {
	if (heap_esta_vacio(heap)) return NULL;
	void* elemento_a_devolver = heap_ver_max(heap);
	heap->cantidad--;
	
	if (heap->cantidad < heap->tamanio/REDUCIR) {
		if(!heap_redimensionar(heap, (heap->tamanio)/REDUCIR)) return NULL;
	}

	if (heap_cantidad(heap) >= 1) {
		heap->vector[0] = heap->vector[heap_cantidad(heap)];
		downheap(heap->vector, heap->cantidad, 0, heap->cmp);
	}

	return elemento_a_devolver;
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)) {
	while(!heap_esta_vacio(heap)) {
		void* elemento = heap_desencolar(heap);
		
		if(destruir_elemento) {
			destruir_elemento(elemento);
		}
	}

	free(heap->vector);
	free(heap);
}