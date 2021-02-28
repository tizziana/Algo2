#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h> 
#include "pila.c"

/*Controla el estado de cada linea, devolviendo true si esta balanceada, y false si no lo esta*/
bool syntax_validation(pila_t* pila, bool estado, char* linea, char* abrir, char* cerrar){
	size_t longitud_linea = strlen(linea);
	for (size_t i = 0; i < longitud_linea; i++) {
		
		if (linea[i] == abrir[0] || linea[i] == abrir[1] || linea[i] == abrir[2]) {
			if (i == strlen(linea) - 2) estado = false;
			pila_apilar(pila, &linea[i]);
		}
		
		else if (linea[i] == cerrar[0] || linea[i] == cerrar[1] || linea[i] == cerrar[2]) {
			if (pila_esta_vacia(pila)) {
				estado = false;
			}

			else {
				int posicion = 0;
				if (linea[i] == cerrar[1]) posicion = 1;
				else if (linea[i] == cerrar[2]) posicion = 2;

				char* elemento = pila_desapilar(pila);
				if ((linea[i] == cerrar[posicion]) && (elemento[0] != (int)abrir[posicion])) estado = false;
			}
		}

		else if (linea[i] == *"'") {
			i++;
			while (linea[i] != '\0' && linea[i] != *"'") {
				if (i == strlen(linea) -1) estado = false;
				i++;
			}
		}
	}

	if (!pila_esta_vacia(pila)) estado = false;
	while(!pila_esta_vacia(pila)) pila_desapilar(pila);
	return estado;
}

/*Dado un archivo pasado por parametro, devuelve OK si los parentesis, corchetes
y/o llaves estan balanceados, y devuelve ERROR en caso contrario.*/
int main(int argc,char* argv[]){
	if (argc != 1){
		fprintf(stderr,"Cantidad de parametros erronea.\n");
		return 1;
	}

	char* abrir = "([{";
	char* cerrar = ")]}";
	pila_t* pila = pila_crear();
	char* linea = NULL; size_t capacidad = 0; ssize_t leidos;
	bool estado;

	while((leidos = getline(&linea,&capacidad,stdin)) > 0) {
		estado = true;
		bool balanceado = syntax_validation(pila, estado, linea, abrir, cerrar);

		if (balanceado) printf("OK\n");
		else printf("ERROR\n");
	}

	free(linea);
	pila_destruir(pila);
	return 0;
}