#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "cola.c"

/* Dado el nombre de un archivo y un número entero n pasado por parámetro,
imprime las ultimas n lineas del archivo. */
int main(int argc,char* argv[]) {
	if (argc != 2){
		fprintf(stderr,"Cantidad de parametros erronea\n");
		return 1;
	}
	if (!isdigit(*argv[1])) {
		fprintf(stderr, "Tipo de parametro incorrecto\n");
		return 1;
	}
	size_t n = (size_t)atoi(argv[1]);
	char* linea = NULL; size_t capacidad = 0; ssize_t leidos; int i = 0; cola_t* cola = cola_crear();

	while((leidos = getline(&linea,&capacidad,stdin)) > 0) {
		if (i >= n){
			free(cola_desencolar(cola));
		}
		char* copia = malloc(sizeof(char) * strlen(linea)+1);
		strcpy(copia, linea);
		cola_encolar(cola, copia);
		i++;
	}
	while (!cola_esta_vacia(cola)) {
		char* dato = (char*)cola_desencolar(cola);
		printf("%s", dato);
		free(dato);
	}
	cola_destruir(cola, NULL);
	free(linea);
	return 0;
}