#include "heap.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define VOLUMEN 3000

/* ******************************************************************
 *						FUNCIONES AUXILIARES
 * *****************************************************************/

int cmp(const void* a, const void* b) {
	return strcmp(a, b);
}

/* ******************************************************************
 *						PRUEBAS UNITARIAS
 * *****************************************************************/

void prueba_crear_heap_vacio() {

	printf("-------------PRUEBAS HEAP1-------------\n");

	/*Creamos un heap vacio.*/
	heap_t* heap = heap_crear(NULL);
	print_test("Prueba Heap crear", heap);
	print_test("Prueba Heap no tiene elementos", !heap_cantidad(heap));
	print_test("Prueba Heap esta vacio", heap_esta_vacio(heap));
	print_test("Prueba Heap no tiene maximo", !heap_ver_max(heap));
	print_test("Prueba Heap no se puede desencolar si no hay elementos", !heap_desencolar(heap));
	
	heap_destruir(heap, NULL);
	print_test("Prueba Heap se destruyo", true);

	heap_t* heap_arr = heap_crear_arr(NULL, 0, NULL);
	print_test("Prueba Heap arr crear", heap_arr);
	print_test("Prueba Heap arr no tiene elementos", !heap_cantidad(heap_arr));
	print_test("Prueba Heap arr esta vacio", heap_esta_vacio(heap_arr));
	print_test("Prueba Heap arr no tiene maximo", !heap_ver_max(heap_arr));
	print_test("Prueba Heap arr no se puede desencolar si no hay elementos", !heap_desencolar(heap_arr));
	
	heap_destruir(heap_arr, NULL);
	print_test("Prueba Heap arr se destruyo", true);

}

void prueba_elementos_estaticos() {

	printf("-------------PRUEBAS HEAP2-------------\n");

	/*Creamos un heap vacio.*/
	heap_t* heap = heap_crear(cmp);
	print_test("Prueba Heap crear", heap);
	print_test("Prueba Heap esta vacio", heap_esta_vacio(heap));

	void* elementos[] = {"6", "5", "3", "7", "8", "2", "99", "1"};

	/*Encolamos elementos en el heap, y verificamos.*/
	print_test("Prueba Heap encolar elemento 6", heap_encolar(heap, elementos[0]));
	print_test("Prueba Heap no esta vacio", !heap_esta_vacio(heap));
	print_test("Prueba Heap tiene 1 elemento", heap_cantidad(heap) == 1);
	print_test("Prueba Heap encolar elemento 5", heap_encolar(heap, elementos[1]));
	print_test("Prueba Heap tiene como maximo el 6", heap_ver_max(heap) == elementos[0]);
	print_test("Prueba Heap encolar elemento 3", heap_encolar(heap, elementos[2]));
	print_test("Prueba Heap tiene 3 elementos", heap_cantidad(heap) == 3);
	print_test("Prueba Heap tiene como maximo el 6", heap_ver_max(heap) == elementos[0]);
	print_test("Prueba Heap encolar elemento 7", heap_encolar(heap, elementos[3]));
	print_test("Prueba Heap tiene como maximo el 7", heap_ver_max(heap) == elementos[3]);
	print_test("Prueba Heap encolar elemento 8", heap_encolar(heap, elementos[4]));
	print_test("Prueba Heap tiene como maximo el 8", heap_ver_max(heap) == elementos[4]);
	print_test("Prueba Heap encolar elemento 2", heap_encolar(heap, elementos[5]));
	print_test("Prueba Heap encolar elemento 99", heap_encolar(heap, elementos[6]));
	print_test("Prueba Heap encolar elemento 1", heap_encolar(heap, elementos[7]));
	print_test("Prueba Heap tiene como maximo el 99", heap_ver_max(heap) == elementos[6]);
	print_test("Prueba Heap tiene 8 elementos", heap_cantidad(heap) == 8);

	/*Desencolo todos los elementos del heap verificando que funcione como corresponde.*/
	print_test("Prueba Heap desencolar 99", heap_desencolar(heap) == elementos[6]);
	print_test("Prueba Heap tiene como maximo el 8", heap_ver_max(heap) == elementos[4]);
	print_test("Prueba Heap desencolar 8", heap_desencolar(heap) == elementos[4]);
	print_test("Prueba Heap tiene como maximo el 7", heap_ver_max(heap) == elementos[3]);
	print_test("Prueba Heap desencolar 7", heap_desencolar(heap) == elementos[3]);
	print_test("Prueba Heap tiene como maximo el 6", heap_ver_max(heap) == elementos[0]);
	print_test("Prueba Heap desencolar 6", heap_desencolar(heap) == elementos[0]);
	print_test("Prueba Heap no esta vacio", !heap_esta_vacio(heap));
	print_test("Prueba Heap desencolar 5", heap_desencolar(heap) == elementos[1]);
	print_test("Prueba Heap tiene como maximo el 3", heap_ver_max(heap) == elementos[2]);
	print_test("Prueba Heap desencolar 3", heap_desencolar(heap) == elementos[2]);
	print_test("Prueba Heap tiene como maximo el 2", heap_ver_max(heap) == elementos[5]);
	print_test("Prueba Heap desencolar 2", heap_desencolar(heap) == elementos[5]);
	print_test("Prueba Heap tiene como maximo el 1", heap_ver_max(heap) == elementos[7]);
	print_test("Prueba Heap desencolar 1", heap_desencolar(heap) == elementos[7]);
	print_test("Prueba Heap tiene no tiene maximo", !heap_ver_max(heap));
	print_test("Prueba Heap esta vacio", heap_esta_vacio(heap));
	
	heap_destruir(heap, NULL);
	print_test("Prueba Heap se destruyo", true);

	heap_t* heap_arr = heap_crear_arr(elementos, 8, cmp);
	print_test("Prueba Heap arr crear", heap_arr);
	print_test("Prueba Heap arr no esta vacio", !heap_esta_vacio(heap_arr));

	print_test("Prueba Heap arr desencolar 99", heap_desencolar(heap_arr) == elementos[6]);
	print_test("Prueba Heap arr tiene como maximo el 8", heap_ver_max(heap_arr) == elementos[4]);
	print_test("Prueba Heap arr desencolar 8", heap_desencolar(heap_arr) == elementos[4]);
	print_test("Prueba Heap arr tiene como maximo el 7", heap_ver_max(heap_arr) == elementos[3]);
	print_test("Prueba Heap arr desencolar 7", heap_desencolar(heap_arr) == elementos[3]);
	print_test("Prueba Heap arr tiene como maximo el 6", heap_ver_max(heap_arr) == elementos[0]);
	print_test("Prueba Heap arr desencolar 6", heap_desencolar(heap_arr) == elementos[0]);
	print_test("Prueba Heap arr no esta vacio", !heap_esta_vacio(heap_arr));
	print_test("Prueba Heap arr desencolar 5", heap_desencolar(heap_arr) == elementos[1]);
	print_test("Prueba Heap arr tiene como maximo el 3", heap_ver_max(heap_arr) == elementos[2]);
	print_test("Prueba Heap arr desencolar 3", heap_desencolar(heap_arr) == elementos[2]);
	print_test("Prueba Heap arr tiene como maximo el 2", heap_ver_max(heap_arr) == elementos[5]);
	print_test("Prueba Heap arr desencolar 2", heap_desencolar(heap_arr) == elementos[5]);
	print_test("Prueba Heap arr tiene como maximo el 1", heap_ver_max(heap_arr) == elementos[7]);
	print_test("Prueba Heap arr desencolar 1", heap_desencolar(heap_arr) == elementos[7]);
	print_test("Prueba Heap arr tiene no tiene maximo", !heap_ver_max(heap_arr));
	print_test("Prueba Heap arr esta vacio", heap_esta_vacio(heap_arr));
	
	heap_destruir(heap_arr, NULL);
	print_test("Prueba Heap arr se destruyo", true);
}


void prueba_elementos_dinamicos() {

	printf("-------------PRUEBAS HEAP3-------------\n");
	
	/*Creamos un heap vacio.*/
	heap_t *heap = heap_crear(cmp);
	print_test("Prueba Heap crear", heap);
	int* elemento1 = malloc(sizeof(int));
	int* elemento2 = malloc(sizeof(int));
	int* elemento3 = malloc(sizeof(int));
	int* elemento4 = malloc(sizeof(int));
	*elemento1 = 3;
	*elemento2 = 2;
	*elemento3 = 4;
	*elemento4 = 27;

	/*Encolamos los elementos dinamicos y luego destruimos el heap, que destruye 
	los elementos.*/
	print_test("Prueba heap insertar 3", heap_encolar(heap, elemento1));
	print_test("Prueba heap insertar 2", heap_encolar(heap, elemento2));
	print_test("Prueba heap insertar 4", heap_encolar(heap, elemento3));
	print_test("Prueba heap ver maximo es 4", heap_ver_max(heap) == elemento3);
	print_test("Prueba heap la cantidad de elementos es 3", heap_cantidad(heap) == 3);
	print_test("Prueba heap esta vacio es false", !heap_esta_vacio(heap));
	print_test("Prueba heap insertar 4", heap_encolar(heap, elemento4));
	print_test("Prueba heap ver maximo es 27", heap_ver_max(heap) == elemento4);
	print_test("Prueba Heap desencolar 27", heap_desencolar(heap) == elemento4);
	free(elemento4);
	heap_destruir(heap, free);
	print_test("Prueba heap se destruyo", true);

	int* elemento5 = malloc(sizeof(int));
	int* elemento6 = malloc(sizeof(int));
	int* elemento7 = malloc(sizeof(int));
	int* elemento8 = malloc(sizeof(int));
	*elemento5 = 3;
	*elemento6 = 2;
	*elemento7 = 4;
	*elemento8 = 27;

	void* arreglo[4] = {elemento5, elemento6, elemento7};
	heap_t *heap_arr = heap_crear_arr(arreglo, 3, cmp);
	print_test("Prueba Heap arr crear", heap);
	print_test("Prueba heap arr ver maximo es 4", heap_ver_max(heap_arr) == elemento7);
	print_test("Prueba heap arr la cantidad de elementos es 3", heap_cantidad(heap_arr) == 3);
	print_test("Prueba heap arr esta vacio es false", !heap_esta_vacio(heap_arr));
	print_test("Prueba heap arr insertar 4", heap_encolar(heap_arr, elemento8));
	print_test("Prueba heap arr ver maximo es 27", heap_ver_max(heap_arr) == elemento8);
	print_test("Prueba Heap arr desencolar 27", heap_desencolar(heap_arr) == elemento8);
	free(elemento8);
	heap_destruir(heap_arr, free);
	print_test("Prueba heap arr se destruyo", true);
}

void prueba_volumen() {
	printf("--------------PRUEBA HEAP4--------------\n");
	
	/*Creamos un heap vacio.*/
	heap_t* heap = heap_crear(cmp);
	print_test("Prueba Heap crear", heap);
	int elementos[VOLUMEN];
	void* arreglo[VOLUMEN];

	/*Encolamos 3000 elementos en el heap.*/
	bool estado = true;
	for(int i = 0; i < VOLUMEN; i++) {	
		elementos[i] = rand() %  VOLUMEN;
		arreglo[i] = &elementos[i];
		estado &= (heap_encolar(heap, &elementos[i]) && (heap_cantidad(heap) == i + 1));
	}

	print_test("Prueba Heap encolar y cantidad muchos elementos", estado);
	print_test("Prueba heap no esta vacio", !heap_esta_vacio(heap));
	heap_t* heap_arr = heap_crear_arr(arreglo, VOLUMEN, cmp);
	print_test("Prueba heap arr crear muchos elementos", heap_arr);
	print_test("Prueba heap arr cantidad muchos elementos", heap_cantidad(heap_arr) == VOLUMEN);
	estado = true;

	for(int i = 0; i < VOLUMEN; i++) {	
		estado &= ((heap_ver_max(heap) == heap_desencolar(heap)) && (heap_cantidad(heap) == VOLUMEN - i - 1));
	}

	print_test("Prueba heap desencolar, ver maximo y cantidad muchos elementos", estado);
	print_test("Prueba heap esta vacio", heap_esta_vacio(heap));
	heap_destruir(heap, NULL);
	print_test("Prueba heap destruir", true);
	estado = true;

	for (int i = 0; i < VOLUMEN; i++) {
		estado &= ((heap_ver_max(heap_arr) == heap_desencolar(heap_arr)) && (heap_cantidad(heap_arr) == VOLUMEN - i - 1));
	}

	print_test("Prueba heap arr desencolar, ver maximo y cantidad muchos elementos", estado);
	print_test("Prueba heap arr esta vacio", heap_esta_vacio(heap_arr));
	heap_destruir(heap_arr, NULL);
	print_test("Prueba heap arr destruir", true);
}

void prueba_heap_sort() {
	printf("------------PRUEBAS HEAP5-----------\n");

	/*Creamos un vector con numeros desordenados, le aplicamos heapsort y vemos como queda ordenado in-place.*/
	int numeros[] = {10, 20, 30, 40, 50, 60, 70, 80, 90};
	void* esperado[]= {&numeros[0], &numeros[1], &numeros[2], &numeros[3], &numeros[4], &numeros[5], &numeros[6], &numeros[7], &numeros[8]};
	void* vector[] = {&numeros[5], &numeros[8], &numeros[2], &numeros[7], &numeros[3], &numeros[6], &numeros[1], &numeros[4], &numeros[0]};
	heap_sort(vector, 9, cmp);

	bool estado = true;
	for(size_t i = 0; i < 9; i++) {
		if(vector[i] != esperado[i]) estado = false;
	}

	/*Verificamos que se haya ordenado correctamente*/
	print_test("Prueba heapsort", estado);
}


/* ******************************************************************
 *						FUNCIÓN PRINCIPAL
 * *****************************************************************/
void pruebas_heap_alumno() {
	/*Ejecuta todas las pruebas unitarias.*/
	prueba_crear_heap_vacio();
	prueba_elementos_estaticos();
	prueba_elementos_dinamicos();
	prueba_volumen();
	prueba_heap_sort();
}

