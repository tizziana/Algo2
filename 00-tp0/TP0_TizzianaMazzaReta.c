#include "tp0.h"

/* ******************************************************************
 *                     FUNCIONES A COMPLETAR
 * *****************************************************************/

/* swap() intercambia dos valores enteros.
 */
void swap (int* x, int* y) {
	
	int puntero_aux = *x;
	
	*x = *y;
	*y = puntero_aux;
} 

/* maximo() busca el mayor elemento del arreglo y devuelve su posicion.
 * Si el vector es de largo 0, devuelve -1.
 */
int maximo(int vector[], int n) {

	if (n > 0){
		
		int posicion_maximo = 0;

		for (int i = 1; i < n; i++) {
			if (vector[i] > vector[posicion_maximo]) {
				posicion_maximo = i;
			}
		}

		return posicion_maximo;
	}

	return -1;
}

/* La función comparar recibe dos vectores y sus respectivas longitudes
 * y devuelve -1 si el primer vector es menor que el segundo; 0 si son
 * iguales; y 1 si el segundo es menor.
 *
 * Un vector es menor a otro cuando al compararlos elemento a elemento,
 * el primer elemento en el que difieren no existe o es menor.
 */
int comparar(int vector1[], int n1, int vector2[], int n2) {

	int j = 0;

	for (int i = 0; i < n1 && i < n2; i++) {

		if (vector1[i] < vector2[i]) {
			return -1;
		}

		else if (vector1[i] > vector2[i]) {
			return 1;
		}
	
	j = i;
	}

	if (n1 != 0 && n2 != 0){
		j++;
	}

	if (j < n2) {
		return -1;
	}

	else if (j < n1) {
		return 1;
	} 

	return 0;
}


/* selection_sort() ordena el arreglo recibido mediante el algoritmo de
 * selección.
 */
void seleccion(int vector[], int n) {
	
	for (int j = n - 1; j > 0; j--){
		
		int pos_max = maximo(vector, j + 1);
		
		int aux = vector[j];
		vector[j] = vector[pos_max];
		vector[pos_max] = aux;

	}

}
