#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* Dado dos archivos pasados por parámetro, e imprime las lineas que son 
diferentes  entre un archivo y el otro.*/
int main(int argc,char* argv[]){
	if (argc != 3){
		fprintf(stderr,"Cantidad de parametros erronea\n");
		return 1;
	}
	FILE* archivo1 = fopen(argv[1], "r");
	FILE* archivo2 = fopen(argv[2], "r");

	if (archivo1 == NULL || archivo2 == NULL) {
		fprintf(stderr,"Archivo erroneo\n");
		return 1;
	}
	char* linea1 = NULL;char* linea2 = NULL; size_t capacidad = 0; ssize_t leidos1; ssize_t leidos2; int i = 1;

	while (true){
		leidos1 = getline(&linea1,&capacidad,archivo1); leidos2 = getline(&linea2,&capacidad,archivo2); 
		if ((leidos1 < 0) && (leidos2 < 0)) break;

		if (strcmp(linea1, linea2) != 0) {
			if (leidos1 <= 0) {
				printf("Diferencia en linea %d \n<\n---\n> %s", i, linea2);
			}
			else if (leidos2 <= 0) {
				printf("Diferencia en linea %d \n< %s---\n>\n", i, linea1);
			}
			else {
				printf("Diferencia en linea %d \n< %s---\n> %s", i, linea1, linea2);
			}
		}
		i++;
	}

	free(linea1);
	free(linea2);
	fclose(archivo1);
	fclose(archivo2);
	return 0;
}