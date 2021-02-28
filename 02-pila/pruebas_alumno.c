#include "pila.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>


/* ******************************************************************
 *                   PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/

void pruebas_pila_alumno() {

	printf("Inicio de pruebas con la pila1:\n");

	/*Creo la pila1, que tiene capacidad 6*/
	pila_t* pila1 = pila_crear();
	print_test("Creo la pila1", pila1 != NULL);
	print_test("La pila1 esta vacia", pila_esta_vacia(pila1) == true);

	/*Apilo elementos, y cada vez que sucede esto compruebo que el tope coincide con lo apilado.*/
	int elementos [] = {10, 20, 25, 30, 40, 50};
	print_test("Apilo el elemento '10' a la pila1", pila_apilar(pila1, &elementos[0]) == true);
	print_test("El tope de la pila1 es '10'", pila_ver_tope(pila1) == &elementos[0]);

	print_test("Apilo el elemento '20' a la pila1", pila_apilar(pila1, &elementos[1]) == true);
	print_test("El tope de la pila1 es '20'", pila_ver_tope(pila1) == &elementos[1]);

	/*Apilo el elemento '25', lo desapilo, y  compruebo que el elemento desapilado coincide.*/
	print_test("Apilo el elemento '25' a la pila1", pila_apilar(pila1, &elementos[2]));
	print_test("Desapilo el ultimo elemento", pila_desapilar(pila1) == &elementos[2]);
	print_test("El tope vuelve a ser '20'", pila_ver_tope(pila1) == &elementos[1]);

	print_test("Apilo el elemento '30' a la pila1", pila_apilar(pila1, &elementos[3]));
	print_test("El tope de la pila1 es '30'", pila_ver_tope(pila1) == &elementos[3]);

	print_test("Apilo el elemento '40' a la pila1", pila_apilar(pila1, &elementos[4]));
	print_test("El tope de la pila1 es '40'", pila_ver_tope(pila1) == &elementos[4]);

	print_test("Apilo el elemento '50' a la pila1", pila_apilar(pila1, &elementos[5]));
	print_test("El tope de la pila1 es '50'", pila_ver_tope(pila1) == &elementos[5]);

	/*Desapilo todos los elementos de la pila1, comparando el tope cada vez que lo hago. Mientras veo si esta vacia o no.*/
	/*Ademas compruebo como la capacidad de la pila1, a medida que desapilamos, disminuye.*/
	print_test("Desapilo el ultimo elemento de la pila1 que es '50'", pila_desapilar(pila1) == &elementos[5]);
	
	print_test("La pila1 no esta vacia", pila_esta_vacia(pila1) == false);
	
	print_test("Ultimo elemento de la pila1 es '40'", pila_ver_tope(pila1) == &elementos[4]);
	print_test("Desapilo el ultimo elemento de la pila1 que es '40'", pila_desapilar(pila1) == &elementos[4]);

	print_test("Ultimo elemento de la pila1 es '30'", pila_ver_tope(pila1) == &elementos[3]);
	print_test("Desapilo el ultimo elemento de la pila1 que es '30'", pila_desapilar(pila1) == &elementos[3]);
	
	print_test("La pila1 no esta vacia", pila_esta_vacia(pila1) == false);

	print_test("Ultimo elemento de la pila1 es '20'", pila_ver_tope(pila1) == &elementos[1]);
	print_test("Desapilo el ultimo elemento de la pila1 que es '20'", pila_desapilar(pila1) == &elementos[1]);

	print_test("Ultimo elemento de la pila1 es '10'", pila_ver_tope(pila1) == &elementos[0]);
	print_test("Desapilo el ultimo elemento de la pila1 que es '10'", pila_desapilar(pila1) == &elementos[0]);

	/*La pila ahora se comporta como una pila recien creada.*/
	print_test("La pila1 esta vacia", pila_esta_vacia(pila1) == true);
	print_test("Una pila vacia no tiene tope", pila_ver_tope(pila1) == NULL);
	print_test("No se puede desapilar una pila vacia", pila_desapilar(pila1) == NULL);

	/*Destruyo la pila1*/
	pila_destruir(pila1);
	print_test("La pila1 fue destruida", true);

	printf("------------------------------------------------------------------\n");
	printf("Inicio de pruebas con la pila2:\n");

	/*Creo la pila2, que tiene capacidad 6*/
	pila_t* pila2 = pila_crear();
	print_test("Creo la pila2", pila2 != NULL);
	print_test("La pila2 esta vacia", pila_esta_vacia(pila2) == true);

	/*Comportamiento de una pila recien creada para ver_tope y desapilar.*/
	print_test("Una pila recien creada no tiene tope", pila_ver_tope(pila2) == NULL);
	print_test("No se puede desapilar una pila recien creada", pila_desapilar(pila2) == NULL);

	/*Apilo una cantidad de elementos que supera ampliamente el valor de la capacidad,
	comprobando como este duplica su tamaño cada vez que se requiere apilar cuando esta "llena".*/
	int i;
	for(i = 1; i <= 30; i++){
		pila_apilar(pila2, &i);
	}
	print_test("Apilo en la pila2 los elementos del 1 al 30", true);
	print_test("El tope es 30.", pila_ver_tope(pila2) == &i);
	
	/*Desapilo elementos y compruebo, cada vez que esto sucede, si el tope coincide.*/
	print_test("Desapilo el ultimo elemento de la pila2 que es '30'", pila_desapilar(pila2) == &i);
	i--;
	print_test("El tope de la pila2 es 29", pila_ver_tope(pila2) == &i);
	print_test("Desapilo el elemento 29 de la pila2", pila_desapilar(pila2) == &i);
	i--;
	print_test("El tope de la pila2 es 28", pila_ver_tope(pila2) == &i);
	print_test("Desapilo el elemento 28 de la pila2", pila_desapilar(pila2) == &i);
	i--;
	print_test("El tope de la pila2 es 27", pila_ver_tope(pila2) == &i);
	print_test("Desapilo el elemento 27 de la pila2", pila_desapilar(pila2) == &i);
	i--;
	print_test("El tope de la pila2 es 26", pila_ver_tope(pila2) == &i);
	print_test("Desapilo el elemento 26 de la pila2", pila_desapilar(pila2) == &i);

	/*Destruyo la pila2*/
	pila_destruir(pila2);
	print_test("Destruyo la pila2", true);

	printf("------------------------------------------------------------------\n");
	printf("Inicio de pruebas con la pila3:\n");

	/*Creo la pila3, que tiene capacidad 6*/
	pila_t* pila3 = pila_crear();
	print_test("Creo la pila2", pila3 != NULL);
	print_test("La pila3 esta vacia", pila_esta_vacia(pila3) == true);

	/*Apilo el elemento nulo y veo si coincide el tope de la pila3.*/
	print_test("Apilo el elemento nulo", pila_apilar(pila3, NULL));
	print_test("El tope de la pila3 es el elemento nulo", pila_ver_tope(pila3) == NULL);

	/*Destruyo la pila3*/
	pila_destruir(pila3);
	print_test("Destruyo la pila3", true);



}
