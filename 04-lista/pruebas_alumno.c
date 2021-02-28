#include "lista.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#define MAXIMO 550


void destruir_dato(void* dato){
	free(dato);
}

bool sumar_todos(void* dato, void* extra) {
	*(int*) extra += *(int*) dato;
	return true;
}

void pruebas_lista_alumno() {

	/*Probamos las primitivas de la lista.*/
	printf("Inicio de pruebas con la lista1:\n");

	/*Creamos la lista1.*/
	lista_t* lista1 = lista_crear();

	/*Vemos si cumple con el comportamiento de una lista recien creada.*/
	print_test("Prueba Lista1 crear", lista1 != NULL);
	print_test("Prueba Lista1 esta vacia inicialmente", lista_esta_vacia(lista1) == true);
	print_test("Prueba Lista1 borrar lista vacia es NULL", lista_borrar_primero(lista1) == NULL);
	print_test("Prueba Lista1 ver largo igual a 0", lista_largo(lista1) == 0);	

	float valores1[]= {50, 100, 150};

	/*Insertamos un elemento con la funcion lista_insertar_ultimo en la lista vacia, vemos si cumple con el primero de la
	lista, con el ultimo, y comprobamos que la lista no esta vacia.*/
	print_test("Prueba Lista1 insertar elemento '100' al final", lista_insertar_ultimo(lista1, &valores1[1]) == true);
	print_test("Prueba Lista1 ver primer elemento", lista_ver_primero(lista1) == &valores1[1]);
	print_test("Prueba Lista1 ver ultimo elemento", lista_ver_ultimo(lista1) == &valores1[1]);
	print_test("Prueba Lista1 no esta vacia", lista_esta_vacia(lista1) == false);


	/*Insertamos dos elementos mas y comprobamos si cumplen.*/
	print_test("Prueba Lista1 insertar elemento '50' al inicio", lista_insertar_primero(lista1, &valores1[0]));
	print_test("Prueba Lista1 insertar elemento '150' al final", lista_insertar_ultimo(lista1, &valores1[2]));
	print_test("Prueba Lista1 ver primer elemento", lista_ver_primero(lista1) == &valores1[0]);
	print_test("Prueba Lista1 ver ultimo elemento", lista_ver_ultimo(lista1) == &valores1[2]);
	print_test("Prueba Lista1 ver largo igual a 3", lista_largo(lista1) == 3);
	print_test("Prueba Lista1 borrar primer elemento", lista_borrar_primero(lista1) == &valores1[0]);
	print_test("Prueba Lista1 ver primer elemento", lista_ver_primero(lista1) == &valores1[1]);
	print_test("Prueba Lista1 borrar primer elemento", lista_borrar_primero(lista1) == &valores1[1]);
	print_test("Prueba Lista1 ver primer elemento", lista_ver_primero(lista1) == &valores1[2]);
	print_test("Prueba Lista1 borrar primer elemento", lista_borrar_primero(lista1) == &valores1[2]);
	print_test("Prueba Lista1 esta vacia", lista_esta_vacia(lista1) == true);
	print_test("Prueba Lista1 ver largo igual a 0", lista_largo(lista1) == 0);

	/*Destruimos la lista1*/
	lista_destruir(lista1, NULL);
	print_test("Prueba Lista1 destruir", true);

	printf("---------------------------------------\n");
	printf("Inicio  de pruebas con la lista2\n");

	/*Creamos la lista2.*/
	lista_t* lista2 =lista_crear();

	/*Vemos si cumple con el comportamiento de una lista recien creada.*/
	print_test("Prueba Lista2 crear", lista2 != NULL);
	print_test("Prueba Lista2 esta vacia inicialmente", lista_esta_vacia(lista2) == true);
	print_test("Prueba Lista2 borrar lista vacia es NULL", lista_borrar_primero(lista2) == NULL);
	print_test("Prueba Lista2 ver largo igual a 0", lista_largo(lista2) == 0);
	print_test("Prueba Lista2 primer elemento es NULL", lista_ver_primero(lista2) == NULL);
	print_test("Prueba Lista2 ultimo elemento es NULL", lista_ver_ultimo(lista2) == NULL);


	double valores2[] = {5, 4, 3, 2, 1};

	/*Insertamos 5 valores a la lista con la funcion lista_insertar_primero.*/
	print_test("Prueba Lista2 insertar elemento '5' al inicio", lista_insertar_primero(lista2, &valores2[0]));
	print_test("prueba Lista2 ver primero", lista_ver_primero(lista2) == &valores2[0]);
	print_test("prueba Lista2 ver ultimo", lista_ver_ultimo(lista2) == &valores2[0]);
	print_test("Prueba Lista2 insertar elemento '4' al inicio", lista_insertar_primero(lista2, &valores2[1]));
	print_test("Prueba Lista2 insertar elemento '2' al inicio", lista_insertar_primero(lista2, &valores2[3]));
	print_test("Prueba Lista2 ver primer elemento", lista_ver_primero(lista2) == &valores2[3]);
	print_test("Prueba Lista2 insertar elemento '1' al inicio", lista_insertar_primero(lista2, &valores2[4]));
	print_test("Prueba Lista2 ver primer elemento", lista_ver_primero(lista2) == &valores2[4]);
	print_test("Prueba Lista2 ver ultimo elemento", lista_ver_ultimo(lista2) == &valores2[0]);

	/*Creamos el iterador externo2 para la lista2.*/
	lista_iter_t* iter2 = lista_iter_crear(lista2);
	print_test("Prueba Iterador2 crear iterador externo", iter2 != NULL);

	/*Vemos si el dato apuntado por el iterador actualmente es el primero de la lista2.*/
	print_test("Prueba Iterador2 ver actual igual a '1'", lista_iter_ver_actual(iter2) == &valores2[4]);
	print_test("Prueba Iterador2 avanzar", lista_iter_avanzar(iter2) == true);
	print_test("Prueba Iterador2 ver actual igual a '2'", lista_iter_ver_actual(iter2) == &valores2[3]);
	print_test("Prueba Iterador2 avanzar", lista_iter_avanzar(iter2) == true);
	print_test("Prueba Iterador2 ver actual igual a '4'", lista_iter_ver_actual(iter2) == &valores2[1]);
	/*Insertamos un elemento en la mitad de la lista con el iterador externo.*/
	print_test("Prueba Iterador2 insertar elemento '3'", lista_iter_insertar(iter2, &valores2[2]));
	print_test("Prueba Iterador2 ver actual igual a '3'", lista_iter_ver_actual(iter2) == &valores2[2]);
	/*Borramos un elemento de la mitad de la lista con el iterador externo.*/
	print_test("Prueba Iterador2 avanzar", lista_iter_avanzar(iter2) == true);
	print_test("Prueba Iterador2 ver actual igual a '4'", lista_iter_ver_actual(iter2) == &valores2[1]);
	print_test("Prueba Iterador2 borrar elemeto actual de la lista2", lista_iter_borrar(iter2) == &valores2[1]);
	print_test("Prueba Iterador2 ver actual igual a '5'", lista_iter_ver_actual(iter2) == &valores2[0]);
	/*Borramos el ultimo elemento de la lista con el iterador externo.*/
	print_test("Prueba Iterador2 borrar ultimo elemento de la lista2", lista_iter_borrar(iter2) == &valores2[0]);
	print_test("Prueba Lista2 ver lagor es 3", lista_largo(lista2) == 3);

	/*Destruimos la lista2 y el iterador externo2.*/
	lista_destruir(lista2, NULL);
	lista_iter_destruir(iter2);
	print_test("Prueba Lista2 destruir", true);

	printf("---------------------------------------\n");
	printf("Inicio  de pruebas con la lista3\n");

	/*Creamos la lista3.*/
	lista_t* lista3 =lista_crear();

	/*Vemos si cumple con el comportamiento de una lista recien creada.*/
	print_test("Prueba Lista3 crear", lista3 != NULL);

	/*Creamos el iterador3 para la lista3, y probamos sus primitivas sobre una lista vacia.*/
	lista_iter_t* iter3 = lista_iter_crear(lista3);
	print_test("Prueba Iterador3 crear iterador externo", iter3 != NULL);
	print_test("Prueba Iterador3 borrar elemento de la lista vacia lista3 es NULL", lista_iter_borrar(iter3) == NULL);
	print_test("Prueba Iterador3 esta al final", lista_iter_al_final(iter3) == true);
	print_test("Prueba Iterador3 avanzar es NULL", lista_iter_avanzar(iter3) == false);
	print_test("Prueba Iterador3 ver actual es NULL", lista_iter_ver_actual(iter3) == NULL);
	char valores3_1[] = "I";
	print_test("Prueba Iterador3 insertar elemento 'I'", lista_iter_insertar(iter3, &valores3_1));

	/*Comprobamos si realmente 'Algoritmos' esta en la lista.*/
	print_test("Prueba Lista3 ver primer elemento es 'I'", lista_ver_primero(lista3) == &valores3_1);
	print_test("Prueba Lista3 ver ultimo elemento es 'I'", lista_ver_ultimo(lista3) == &valores3_1);
	print_test("Prueba Lista3 no esta vacia", lista_esta_vacia(lista3) == false);

	/*Insertamos elementos solo con el iterador externo.*/
	char valores3_2[] = "Algoritmos";
	char valores3_3[] = "y";
	char valores3_4[] = "programacion";

	print_test("Prueba Iterador2 insertar elemento 'Algoritmos'", lista_iter_insertar(iter3, &valores3_2));
	print_test("Prueba Iterador2 ver actual", lista_iter_ver_actual(iter3) == &valores3_2);
	print_test("Prueba Lista3 ver ultimo elemento es 'I'", lista_ver_ultimo(lista3) == &valores3_1);
	print_test("Prueba Iterador4 avanzar", lista_iter_avanzar(iter3) == true);
	print_test("Prueba Iterador2 ver actual", lista_iter_ver_actual(iter3) == &valores3_1);
	print_test("Prueba Iterador2 insertar elemento 'y'", lista_iter_insertar(iter3, &valores3_3));
	print_test("Prueba Iterador2 insertar elemento 'programacion'", lista_iter_insertar(iter3, &valores3_4));
	print_test("Prueba Lista3 ver ultimo elemento es 'I'", lista_ver_ultimo(lista3) == &valores3_1);

	print_test("Prueba Lista3 ver largo es 4", lista_largo(lista3) == 4);

	/*Destruimos el iterador3 y la lista3.*/
	lista_iter_destruir(iter3);
	lista_destruir(lista3, NULL);
	print_test("Prueba Lista3 destruir", true);

	printf("---------------------------------------\n");
	printf("Inicio  de pruebas con la lista4\n");

	/*Creamos la lista4.*/
	lista_t* lista4 =lista_crear();

	int valores4[] = {0, 5, 10, 15};

	print_test("Prueba Lista4 insertar elemento '5' al inicio", lista_insertar_primero(lista4, &valores4[1]));
	print_test("Prueba Lista4 insertar elemento '10' al inicio", lista_insertar_ultimo(lista4, &valores4[2]));
	print_test("Prueba Lista4 ver primer elemento", lista_ver_primero(lista4) == &valores4[1]);
	print_test("Prueba Lista4 ver ultimo elemento", lista_ver_ultimo(lista4) == &valores4[2]);

	lista_iter_t* iter4_1 = lista_iter_crear(lista4);
	print_test("Prueba Iterador4_1 crear iterador externo", iter4_1 != NULL);

	/*Insertamos el elemento '0' en la primera posicion de la lista, con el iterador externo.*/
	print_test("Prueba Iterador4_1 insertar elemento '0'", lista_iter_insertar(iter4_1, &valores4[0]));
	print_test("Prueba Iterador4_1 ver actual igual a '0'", lista_iter_ver_actual(iter4_1) == &valores4[0]);

	print_test("Prueba Iterador4_1 avanzar", lista_iter_avanzar(iter4_1) == true);
	print_test("Prueba Iterador4_1 ver actual igual a '5'", lista_iter_ver_actual(iter4_1) == &valores4[1]);
	print_test("Prueba Iterador4_1 avanzar", lista_iter_avanzar(iter4_1) == true);
	print_test("Prueba Iterador4_1 ver actual igual a '10'", lista_iter_ver_actual(iter4_1) == &valores4[2]);	
	print_test("Prueba Iterador4_1 avanzar", lista_iter_avanzar(iter4_1) == true);
	print_test("Prueba Iterador4_1 insertar elemento '15'", lista_iter_insertar(iter4_1, &valores4[3]));
	print_test("Prueba Iterador4_1 ver actual igual a '15'", lista_iter_ver_actual(iter4_1) == &valores4[3]);

	print_test("Prueba Lista4 ver largo es 3", lista_largo(lista4) == 4);

	lista_iter_t* iter4_2 = lista_iter_crear(lista4);
	print_test("Prueba Iterador4_2 crear iterador externo", iter4_2 != NULL);

	print_test("Prueba Iterador4_2 ver actual igual a '0'", lista_iter_ver_actual(iter4_2) == &valores4[0]);
	print_test("Prueba Iterador4_2 avanzar", lista_iter_avanzar(iter4_2) == true);
	print_test("Prueba Iterador4_2 ver actual igual a '5'", lista_iter_ver_actual(iter4_2) == &valores4[1]);
	print_test("Prueba Iterador4_2 avanzar", lista_iter_avanzar(iter4_2) == true);
	print_test("Prueba Iterador4_2 ver actual igual a '10'", lista_iter_ver_actual(iter4_2) == &valores4[2]);	
	print_test("Prueba Iterador4_2 avanzar", lista_iter_avanzar(iter4_2) == true);
	print_test("Prueba Iterador4_2 insertar elemento '15'", lista_iter_insertar(iter4_2, &valores4[3]));
	print_test("Prueba Iterador4_2 ver actual igual a '15'", lista_iter_ver_actual(iter4_2) == &valores4[3]);

	/*Probamos el iterador interno, utilizando la funcion sumar_todos.*/
	int suma4 = 0;
	lista_iterar(lista4, sumar_todos, &suma4);
	printf("La suma de todos los elentos de la lista es: %d\n", suma4);

	/*Destruiyo el iterador4 y la lista4.*/
	lista_iter_destruir(iter4_1);
	lista_iter_destruir(iter4_2);
	lista_destruir(lista4, NULL);
	print_test("Prueba Lista4 destruir", true);

	printf("---------------------------------------\n");
	printf("Inicio  de pruebas con la lista5\n");

	/*Creamos la Cola5.*/
	lista_t* lista5 = lista_crear();
	print_test("Prueba Lista5 esta vacia inicialmente", lista_esta_vacia(lista5) == true);
	
	/*Insertamos elementos dinamicos en la lista para probar volumen.*/
	void* valores5[MAXIMO];

	for (int i = 0; i < MAXIMO; i++){
		int* valor5 = malloc(sizeof(int));
		*valor5 = i;
		valores5[i] = valor5;
		lista_insertar_ultimo(lista5, valor5);
	}

	print_test("Prueba Lista5 insertar 550 elementos dinamicos, del 0 al 549", true);

	/*Verificamos que la lista no esta vacia.*/
	print_test("Prueba Lista5 no esta vacia", lista_esta_vacia(lista5) == false);

	/*Borramos algunos elementos, y cada vez que hacemos esto vemos si el primer nuevo elemento cumple.*/
	print_test("Prueba Lista5 ver primer elemento", lista_ver_primero(lista5) == valores5[0]);
	print_test("Prueba Lista5 borrar el elemento '0'", lista_borrar_primero(lista5) == valores5[0]);
	print_test("Prueba Lista5 ver primer elemento", lista_ver_primero(lista5) == valores5[1]);
	print_test("Prueba Lista5 borrar el elemento '1'", lista_borrar_primero(lista5) == valores5[1]);
	print_test("Prueba Lista5 ver primer elemento", lista_ver_primero(lista5) == valores5[2]);
	/*Creamos el iterador5 para borrar algunos elementos*/
	lista_iter_t* iter5 = lista_iter_crear(lista5);
	print_test("Prueba Iterador5 crear iterador externo", iter5 != NULL);
	print_test("Prueba Lista5 borrar el elemento '2'", lista_iter_borrar(iter5) == valores5[2]);
	print_test("Prueba Lista5 ver primer elemento", lista_ver_primero(lista5) == valores5[3]);
	print_test("Prueba Lista5 borrar el elemento '3'", lista_iter_borrar(iter5) == valores5[3]);
	print_test("Prueba Lista5 ver primer elemento", lista_ver_primero(lista5) == valores5[4]);

	/*Volvemos a insertar lo que borramos asi podemos destruirlo junto con la lista*/
	lista_insertar_primero(lista5, valores5[0]);
	lista_insertar_primero(lista5, valores5[1]);
	lista_insertar_primero(lista5, valores5[2]);
	lista_insertar_primero(lista5, valores5[3]);

	/*Destuimos la lista5.*/
	lista_destruir(lista5, destruir_dato);
	lista_iter_destruir(iter5);
	print_test("Prueba Lista5 destuir", true);
	printf("---------------------------------------\n");
	printf("Inicio  de pruebas con la lista6\n");

	/*Creamos la lista6.*/
	lista_t* lista6 = lista_crear();
	print_test("Prueba Lista6 crear", lista6 != NULL);
	print_test("Prueba Lista6 esta vacia inicialmente", lista_esta_vacia(lista6) == true);

	int valores6 [MAXIMO];
	
	for(int i = 0; i < MAXIMO; i++){
		valores6[i] = i;
		lista_insertar_ultimo(lista6, &valores6[i]);
	}

	print_test("Prueba Lista6 insertar 550 elementos estaticos, del 0 al 549", true);
	/*Comprobamos que el primer elemento y el ultimo corresponden.*/
	print_test("Prueba Lista6 ver primer elemento", lista_ver_primero(lista6) == &valores6[0]);
	print_test("Prueba Lista6 ver ultimo elemento", lista_ver_ultimo(lista6) == &valores6[549]);
	
	/*Utilizamos el iterador interno para sumar todos los elementos de la lista*/
	int suma6 = 0;
	lista_iterar(lista6, sumar_todos, &suma6);
	printf("La suma de todos los elementos de la lista es: %d\n", suma6);

	lista_destruir(lista6, NULL);
	print_test("Prueba Lista6 destruir", true);

	printf("---------------------------------------\n");
	printf("Inicio  de pruebas con la lista7\n");

	//Creamos la lista7.
	lista_t* lista7 = lista_crear();
	print_test("Prueba Lista7 crear", lista7 != NULL);

	lista_iter_t* iter7 = lista_iter_crear(lista7);
	print_test("Prueba Iterador7 crear iterador externo", iter7 != NULL);

	int valores7[] = {1, 2, 3};

	print_test("Prueba Iterador7 insertar elemento '1'", lista_iter_insertar(iter7, &valores7[0]));
	print_test("Prueba Iterador7 ver primer elemento", lista_iter_ver_actual(iter7) == &valores7[0]);
	print_test("Prueba Lista3 ver primer elemento es '1'", lista_ver_primero(lista7) == &valores7[0]);
	print_test("Prueba Lista3 ver ultimo elemento es '1'", lista_ver_ultimo(lista7) == &valores7[0]);
	print_test("Prueba Iterador7 insertar elemento '2'", lista_iter_insertar(iter7, &valores7[1]));
	print_test("Prueba Iterador7 insertar elemento '3'", lista_iter_insertar(iter7, &valores7[2]));
	print_test("Prueba Iterador7 ver primer elemento", lista_iter_ver_actual(iter7) == &valores7[2]);

	print_test("Prueba Iterador7 borrar primer elemento de la lista7", lista_iter_borrar(iter7) == &valores7[2]);
	print_test("Prueba Iterador7 borrar primer elemento de la lista7", lista_iter_borrar(iter7) == &valores7[1]);

	lista_destruir(lista7, NULL);
	lista_iter_destruir(iter7);
	print_test("Prueba Lista7 destruir", true);

}
