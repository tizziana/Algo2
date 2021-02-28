#include "cola.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#define MAXIMO 500

void destruir_dato(void* dato){
	free(dato);
}

void pruebas_cola_alumno() {

	printf("Inicio de pruebas con la cola1:\n");

	/*Creamos la Cola1.*/
	cola_t* cola1 = cola_crear();

	/*Vemos si cumple con el comportamiento de una cola recien creada.*/
	print_test("Prueba Cola1 crear", cola1 != NULL);
	print_test("Prueba Cola1 esta vacia inicialmente", cola_esta_vacia(cola1) == true);
	print_test("Prueba Cola1 desencolar cola vacia es NULL", cola_desencolar(cola1) == NULL);

	int valor1 = 20;

	/*Encolamos un elemento, vemos si cumple con el primero de la cola, lo desencolamos
	y vemos si la cola esta vacia.*/
	print_test("Prueba Cola1 encolar elemento 20", cola_encolar(cola1, &valor1) == true);
	print_test("Prueba Cola1 ver primer elemento", cola_ver_primero(cola1) == &valor1);
	print_test("Prueba Cola1 desencolar elemento 20", cola_desencolar(cola1) == &valor1);
	print_test("Prueba Cola1 comprobar que esta vacia luego de desencolar", cola_esta_vacia(cola1) == true);


	/*Destruimos la cola1.*/
	cola_destruir(cola1, NULL);
	print_test("Prueba Cola1 destuir", true);
	
	printf("---------------------------------------\n");
	printf("Inicio de pruebas con la cola2 (con elementos estaticos):\n");

	/*Creo la cola2.*/
	cola_t* cola2 = cola_crear();

	print_test("Prueba Cola2 crear", cola2 != NULL);
	print_test("Prueba Cola2 esta vacia inicialmente", cola_esta_vacia(cola2) == true);

	/*Creamos valores de forma estatica para encolar en la cola2.*/
	int valor_a = 1;
	short valor_b = 2;
	int valor_c = 3;
	double valor_d = 4;
	short valor_e = 5;

	void* valores2[] = {&valor_a, &valor_b, &valor_c, &valor_d, &valor_e};

	print_test("Prueba Cola2 encolar elemento '1'", cola_encolar(cola2, valores2[0]) == true);
	print_test("Prueba Cola2 ver primer elemento", cola_ver_primero(cola2) == valores2[0]);

	print_test("Prueba Cola2 no esta vacia", cola_esta_vacia(cola2) == false);

	print_test("Prueba Cola2 encolar el elemento '2'", cola_encolar(cola2, valores2[1]) == true);
	print_test("Prueba Cola2 ver primer elemento", cola_ver_primero(cola2) == valores2[0]);

	print_test("Prueba Cola2 encolar el elemento '3'", cola_encolar(cola2, valores2[2]) == true);
	print_test("Prueba Cola2 encolar el elemento '4'", cola_encolar(cola2, valores2[3]) == true);
	print_test("Prueba Cola2 encolar el elemento '5'", cola_encolar(cola2, valores2[4]) == true);	

	/*Vamos desencolando los elementos, y viendo si el primer valor cumple.*/
	print_test("Prueba Cola2 desencolar el elemento '1'", cola_desencolar(cola2) == valores2[0]);
	print_test("Prueba Cola2 ver primer elemento", cola_ver_primero(cola2) == valores2[1]);
	print_test("Prueba Cola2 desencolar el elemento '2'", cola_desencolar(cola2) == valores2[1]);
	print_test("Prueba Cola2 ver primer elemento", cola_ver_primero(cola2) == valores2[2]);
	print_test("Prueba Cola2 desencolar el elemento '3'", cola_desencolar(cola2) == valores2[2]);
	print_test("Prueba Cola2 ver primer elemento", cola_ver_primero(cola2) == valores2[3]);
	print_test("Prueba Cola2 desencolar el elemento '4'", cola_desencolar(cola2) == valores2[3]);
	print_test("Prueba Cola2 ver primer elemento", cola_ver_primero(cola2) == valores2[4]);
	print_test("Prueba Cola2 desencolar el elemento '5'", cola_desencolar(cola2) == valores2[4]);

	/*Comprobamos que una cola vacia se comporta como una recien creada.*/
	print_test("Prueba Cola2 desencolar una cola vacia es NULL", cola_desencolar(cola2) == NULL);
	print_test("Prueba Cola2 ver primer elemento de cola vacia es NULL", cola_ver_primero(cola2) == NULL);
	print_test("Prueba Cola2 esta vacia", cola_esta_vacia(cola2) == true);

	/*Destruimos la Cola2.*/
	cola_destruir(cola2, NULL);
	print_test("Prueba Cola2 destuir", true);

	printf("---------------------------------------\n");
	printf("Inicio de pruebas con la cola3 (con elementos dinamicos):\n");

	/*Creamos la cola3.*/
	cola_t* cola3 = cola_crear();

	print_test("Prueba Cola3 crear", cola3 != NULL);
	print_test("Prueba Cola3 esta vacia inicialmente", cola_esta_vacia(cola3) == true);

	/*Creamos elementos de forma dinamica, para luego encolarlos.*/
	int* valor_w = malloc(sizeof(int));
	*valor_w = 10;
	short* valor_x = malloc(sizeof(short));
	*valor_x = 20;
	float* valor_y = malloc(sizeof(float));
	*valor_y = 30;
	int* valor_z = malloc(sizeof(int));
	*valor_z = 40;

	void* valores3[] = {valor_w, valor_x, valor_y, valor_z};

	print_test("Prueba Cola3 encolar elemento '10'", cola_encolar(cola3, valores3[0]) == true);
	print_test("Prueba Cola3 ver el primer elemento", cola_ver_primero(cola3) == valores3[0]);
	print_test("Prueba Cola3 desencolar el elemento '10'", cola_desencolar(cola3) == valores3[0]);
	print_test("Prueba Cola3 no tiene elementos", cola_ver_primero(cola3) == NULL);
	
	print_test("Prueba Cola3 encolar el elemento '20'", cola_encolar(cola3, valores3[1]) == true);
	print_test("Prueba Cola3 ver primer elemento", cola_ver_primero(cola3) == valores3[1]);
	print_test("Prueba Cola3 encolar el elemento '30'", cola_encolar(cola3, valores3[2]) == true);
	print_test("Prueba Cola3 encolar el elemento '40'", cola_encolar(cola3, valores3[3]) == true);

	/*Desencolamos y comprobamos si el primer elemento cumple.*/
	print_test("Prueba Cola3 desencolar el elemento '20'", cola_desencolar(cola3) == valores3[1]);
	print_test("Prueba Cola3 ver primer elemento", cola_ver_primero(cola3) == valores3[2]);
	print_test("Prueba Cola3 desencolar el elemento '30'", cola_desencolar(cola3) == valores3[2]);
	print_test("Prueba Cola3 ver primer elemento", cola_ver_primero(cola3) == valores3[3]);
	print_test("Prueba Cola3 desencolar el elemento '40'", cola_desencolar(cola3) == valores3[3]);
	print_test("Prueba Cola3 no tiene elementos", cola_ver_primero(cola3) == NULL);

	print_test("Prueba Cola3 desencolar cola vacia es NULL", cola_desencolar(cola3) == NULL);
	print_test("Prueba Cola3 esta vacia", cola_esta_vacia(cola3) == true);

	/*Encolamos los elementos que desencolamos para destruirlos y luego destruir la cola.*/
	cola_encolar(cola3, valores3[0]);
	cola_encolar(cola3, valores3[1]);
	cola_encolar(cola3, valores3[2]);
	cola_encolar(cola3, valores3[3]);

	/*Destruimos la Cola3.*/
	cola_destruir(cola3, destruir_dato);
	print_test("Prueba Cola3 destuir", true);

	printf("---------------------------------------\n");
	printf("Inicio de pruebas con la cola4 (con elementos estaticos):\n");

	/*Creamos la cola4.*/
	cola_t* cola4 = cola_crear();
	
	print_test("Prueba Cola4 crear", cola4 != NULL);
	print_test("Prueba Cola4 esta vacia inicialmente", cola_esta_vacia(cola4) == true);

	/*Encolamos elementos en la cola para probar volumen.*/
	void* valores4[MAXIMO];

	for(int i = 0; i < MAXIMO; i++){
		valores4[i] = &i;
		cola_encolar(cola4, valores4[i]);
	}

	print_test("Prueba Cola4 encolar 500 elementos del 0 al 499.", true);

	/*Verificamos que no esta vacia y que el primer elemento cumple.*/
	print_test("Prueba Cola4 no esta vacia", cola_esta_vacia(cola4) == false);
	print_test("Prueba Cola4 ver primer elemento que es '0'", cola_ver_primero(cola4) == valores4[0]);

	/*Desencolamos algunos elementos y vemos el primero para ver si cumple.*/
	print_test("Prueba Cola4 desencolar primer elemento", cola_desencolar(cola4) == valores4[0]);
	print_test("Prueba Cola4 ver primer elemento que es '1'", cola_ver_primero(cola4) == valores4[1]);
	print_test("Prueba Cola4 desencolar primer elemento", cola_desencolar(cola4) == valores4[1]);
	print_test("Prueba Cola4 ver primer elemento que es '2'", cola_ver_primero(cola4) == valores4[2]);
	print_test("Prueba Cola4 desencolar primer elemento", cola_desencolar(cola4) == valores4[2]);
	print_test("Prueba Cola4 ver primer elemento que es '3'", cola_ver_primero(cola4) == valores4[3]);
	print_test("Prueba Cola4 desencolar primer elemento", cola_desencolar(cola4) == valores4[3]);
	print_test("Prueba Cola4 ver primer elemento que es '4'", cola_ver_primero(cola4) == valores4[4]);

	/*Destruimos la Cola4.*/
	cola_destruir(cola4, NULL);
	print_test("Prueba Cola4 destuir", true);

	printf("---------------------------------------\n");
	printf("Inicio de pruebas con la cola5 (con elementos dinamicos):\n");

	/*Creamos la Cola5.*/
	cola_t* cola5 = cola_crear();
	print_test("Prueba Cola5 esta vacia inicialmente", cola_esta_vacia(cola5) == true);
	
	/*Encolamos elementos dinamicos en la cola para probar volumen.*/
	void* valores5[MAXIMO];

	for (int i = 0; i < MAXIMO; i++){
		int* valor5 = malloc(sizeof(int));
		*valor5 = i;
		valores5[i] = valor5;
		cola_encolar(cola5, valor5);
	}

	print_test("Prueba Cola5 encolar 500 elementos del 0 al 499", true);

	/*Verificamos que la cola no esta vacia.*/
	print_test("Prueba Cola5 no esta vacia", cola_esta_vacia(cola5) == false);

	/*Desencolamos algunos elementos, y cada vez que hacemos esto vemos si el primer nuevo elemento cumple.*/
	print_test("Prueba Cola5 ver primer elemento", cola_ver_primero(cola5) == valores5[0]);
	print_test("Prueba Cola5 desencolar el elemento '0'", cola_desencolar(cola5) == valores5[0]);
	print_test("Prueba Cola5 ver primer elemento", cola_ver_primero(cola5) == valores5[1]);
	print_test("Prueba Cola5 desencolar el elemento '1'", cola_desencolar(cola5) == valores5[1]);
	print_test("Prueba Cola5 ver priemr elemento", cola_ver_primero(cola5) == valores5[2]);
	print_test("Prueba Cola5 desencolar el elemento '2'", cola_desencolar(cola5) == valores5[2]);
	print_test("Prueba Cola5 ver priemr elemento", cola_ver_primero(cola5) == valores5[3]);
	print_test("Prueba Cola5 desencolar el elemento '3'", cola_desencolar(cola5) == valores5[3]);
	print_test("Prueba Cola5 ver priemr elemento", cola_ver_primero(cola5) == valores5[4]);


	/*Volvemos a encolar lo que desencolamos asi podemos destruirlo junto con la cola*/
	cola_encolar(cola5, valores5[0]);
	cola_encolar(cola5, valores5[1]);
	cola_encolar(cola5, valores5[2]);
	cola_encolar(cola5, valores5[3]);

	/*Destuimos la Cola5.*/
	cola_destruir(cola5, destruir_dato);
	print_test("Prueba Cola5 destuir", true);

}