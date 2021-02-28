#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include "strutil.h"

char** split(const char* str, char sep){
    char** cadena_partida;
    size_t cant_sep = 1;
    for(int i = 0; i < strlen(str); i++){
        if(str [i] == sep) cant_sep ++;
    }
    cadena_partida = malloc (sizeof (char* )* (cant_sep + 1));
    if (!cadena_partida) return NULL;
    int indice = 0;
    for (int i = 0; i< cant_sep; i++){
        size_t largo = 0;        
	while (str [indice] != sep && str [indice]){
            largo++;
            indice ++;
        }
        char* auxiliar = malloc (sizeof (char) * (largo+1));
        if (!auxiliar) return NULL;
        memcpy (auxiliar, &str[indice-largo], largo);
        auxiliar[largo] = '\0';
        indice++;
        cadena_partida [i] = auxiliar;
    }
    cadena_partida [cant_sep] = NULL;
    return cadena_partida;
}

char* join(char** strv, char sep){
	char* cadena;
	int largo = 1;
	if (!strv[0]) {
        	char * resultado_vacio = calloc(1, sizeof(char));
        	strcpy(resultado_vacio, "");
	       	return resultado_vacio;
	}
	for(int i=0; strv[i]; i++){
		if (!strv[i]) largo++;
		for (int j = 0; strv[i][j]; j++) largo++;
		largo++;
	}
	cadena = calloc(largo+1, sizeof(char));
	if (!cadena) return NULL;
	strcpy(cadena,strv[0]); 	
	size_t actual = strlen(strv[0]);
	for (int i=1; strv[i]; i++){
		cadena[actual] = sep;
		actual++;
		strcat(&cadena[actual],strv[i]);
		actual +=strlen(strv[i]);
	}
	cadena[largo] = '\0';
	return cadena;	
}

void free_strv(char* strv[]) {
	for (int i = 0; strv [i] ; i++) free (strv[i]);
	free (strv);
}	

