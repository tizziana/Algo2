#include "strutil.h"
#include <stdio.h>
#include <string.h> 
#include <stdlib.h>


char* cadena_crear(size_t longitud) {
	char* subcadena = malloc(sizeof(char) * longitud);
	if (!subcadena) return NULL;
	return subcadena;
}
char** agregar_espacio(char** strv, size_t posicion) {
	char* subcadena = cadena_crear(1);
	if (!subcadena) return NULL;
	subcadena[0] = '\0';
	strv[posicion] = subcadena;
	return strv;
}
/****************************************************************************************/
char** split(const char* str, char sep){
	size_t contador_sep = 0;
	size_t longitud_cadena = strlen(str);
	size_t contador = 0; 

	for (int i = 0; i < longitud_cadena; i++) if (str[i] == sep) contador_sep++;

	char** strv = malloc(sizeof(char*) * (contador_sep + 2));
	if (!strv) return NULL;

	if (longitud_cadena == 0 || (longitud_cadena == 1 && contador_sep == 1)){
		agregar_espacio(strv, 0);
		if (longitud_cadena == 1){
			agregar_espacio(strv, 1);
			contador++;
		}
	}
	else{
		size_t l = 0;
		for (size_t j = 0; j < longitud_cadena; j++){
			if ((str[j] == sep && j == 0) || (str[j] == sep && str[j -1] == sep)){
				agregar_espacio(strv, contador);
				contador++;
				l++;
			}
			else if (str[j] == sep || contador == contador_sep) {
				size_t longitud;
				if (str[j] == sep) longitud = j - l;
				if (contador == contador_sep) longitud = longitud_cadena - j;
				char* subcadena = cadena_crear(longitud + 1);
				if (!subcadena) return NULL;
				strncpy(subcadena, &str[l], longitud);
				subcadena[longitud] = '\0';
				strv[contador] = subcadena;
				if (contador == contador_sep) break;
				l = j + 1;
				contador ++;
			}
		}
	}
	if (str[longitud_cadena-1] == sep){
		agregar_espacio(strv, contador);
	}
	strv[contador + 1] = NULL;
	return strv;
}

/****************************************************************************************/
char* join(char** strv, char sep){
	size_t cantidad_cadenas = 0;
	size_t cantidad_caracteres = 0;
	size_t contador = 0;
	size_t longitud = 0;

	for (int i = 0; strv[i] != NULL; i++) {
		cantidad_cadenas++;
		cantidad_caracteres += strlen(strv[i]);
	}

	if ((cantidad_cadenas == 0) || (cantidad_cadenas == 1 && cantidad_caracteres == 0)) longitud = 1;
	else longitud = cantidad_cadenas + cantidad_caracteres;
	
	char* str = cadena_crear(longitud);
	if (!str) return NULL;

	if (cantidad_caracteres == 0 && cantidad_cadenas <= 1){
		str[0] = '\0';
	}
	else {
		for (int l = 0; strv[l] != NULL; l++) {
			strncpy(&str[contador], strv[l], strlen(strv[l]));
			contador+=strlen(strv[l]);
			if (cantidad_cadenas != 1 && l < cantidad_cadenas -1){
				str[contador] = sep;
				contador++;
			}
		str[contador] = '\0';
		}
	}
	return str;
}

void free_strv(char* strv[]){
	for (int i = 0; strv[i] != NULL; i++) {
		free(strv[i]);
	}
	free(strv);
}