#define _XOPEN_SOURCE
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>
#include "funciones_auxiliares.h"
#include "comando.h"
#define TIME_FORMAT "%FT%T%z"
#define CANT_IP 4

time_t iso8601_to_time(const char* iso8601)
{
    struct tm bktime = { 0 };
    strptime(iso8601, TIME_FORMAT, &bktime);
    return mktime(&bktime);
}

//FUNCIONES DE COMPARACION

int comparar_ips(const void* ip1, const void* ip2) {
	char** ip_1 = split(ip1, '.');
	char** ip_2 = split(ip2, '.');
	if (!ip_1 || !ip_2) return 0;
	int resultado = 0;

	for (size_t i = 0; i < CANT_IP; i++) {
		int res_ip1 = (int) strtof(ip_1[i], NULL); //Convertir string a float
		int res_ip2 = (int) strtof(ip_2[i], NULL);

		resultado = res_ip2 - res_ip1;
		if (resultado != 0) break;
	}

	free_strv(ip_1);
	free_strv(ip_2);
	return resultado;
}

//FUNCIONES GENERALES

registro_t* leer_linea (char* linea, size_t cantidad, FILE*archivo){
	linea[strcspn(linea, "\n")] = '\0';
        char** datos_registro = split(linea, '\t');
        registro_t* registro= crear_registro(linea, datos_registro);
        free_strv(datos_registro);
	return registro;
}

//ORDENAR ARCHIVO

char* crear_nombre_particion(const char* nombre, size_t particion) {
    char* nombre_archivo = malloc(sizeof(char) * (strlen(nombre) + 5));
    sprintf(nombre_archivo, "%s_%02zu", nombre, particion);
    return nombre_archivo;
}


FILE* crear_archivo_particion(const char* nombre, size_t particion, char* modo) {
    char* nombre_archivo = crear_nombre_particion(nombre, particion);
    FILE* salida_actual = fopen(nombre_archivo, modo);
    free(nombre_archivo);
    return salida_actual;
}


int destruir_todo_ordenar (const char* nombre, size_t particion, heap_t* heap, lista_t* lista, char* linea){
	for (int i = 0; i < particion ; i++){
		char* nombre = crear_nombre_particion(nombre, particion);
        	remove(nombre);
	}
	heap_destruir(heap, free);
	lista_destruir(lista, free);
	free(linea);
	return -1;
}

//AGREGAR ARCHIVO

int calcular_diferencia (char* tiempo1, char* tiempo5){
    return (int)(difftime(iso8601_to_time(tiempo1), iso8601_to_time(tiempo5)));
}

char* crear_copia (char* fecha){
	char* fecha_copia = malloc(sizeof(char) * (strlen(fecha) + 1));
	if (!fecha_copia) return NULL;
        strcpy(fecha_copia, fecha);
	return fecha_copia;
}

// VER VISITANTES

bool visitar_ip (const char* clave, void* dato, void* extra){
	fprintf(stdout, "\t%s\n", clave);
	return true;
}
