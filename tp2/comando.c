#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>
#include "comando.h"
#include "funciones_auxiliares.h"
#define KILO 1024
#define TAMANIO_LINEA 200
#define CANT_ITER 4
#define CANT_DIF 2

/*--- Estructuras----*/
struct registro{
	char* ip;
	char* fecha;
	char* recurso;
	char* metodo;
	char* linea;
};

typedef struct aux{
	registro_t* datos;
	FILE* archivo;
	int particion;
}aux_t;


registro_t* crear_registro(char* linea, char** datos){

	registro_t* registro = malloc(sizeof(registro_t));
	if (!registro) return NULL;
	registro->ip = strdup(datos[0]);
	registro->fecha = strdup(datos [1]);
	registro->recurso = strdup(datos[3]);
	registro->metodo = strdup (datos[2]);
	registro->linea = strdup(linea);
	return registro;
}

void registro_destruir (registro_t* registro){
	free(registro->ip);
	free(registro->fecha);
	free(registro->recurso);
	free(registro->metodo);
	free(registro->linea);
	free(registro);
}

// AGREGAR ARCHIVOS

void agregar_hash_ataque (hash_t* hash, registro_t* registro){
   // Si el IP no pertenece al Hash, se debe crear una lista nueva. Si ya pertence, se inserta en la lista
   // una COPIA de la fecha
	if (!hash_pertenece(hash, registro->ip)){
		lista_t*lista_de_tiempos = lista_crear();
		if (!lista_de_tiempos) return;
		char* fecha_copia = crear_copia(registro->fecha);
		lista_insertar_ultimo(lista_de_tiempos, fecha_copia);
		hash_guardar(hash, registro->ip, lista_de_tiempos);
		}
		else{
		lista_t* lista_de_tiempos= hash_obtener(hash, registro->ip);
		if (!lista_de_tiempos) return;
		char* fecha_copia = crear_copia(registro->fecha);
		lista_insertar_ultimo(lista_de_tiempos, fecha_copia);
		hash_guardar(hash, registro->ip, lista_de_tiempos);
	}	
}

heap_t* detectar_ataque(hash_t* hash){

	heap_t* heap_ataque = heap_crear(comparar_ips);
	if (!heap_ataque) return NULL;
	hash_iter_t* hash_iter = hash_iter_crear(hash);
	if (!hash_iter) return NULL;

	for ( ; !hash_iter_al_final(hash_iter); hash_iter_avanzar(hash_iter)){
	// Se itera por todo el Hash en busca de ataques DoS, solo itero en aquellas listas que tengan posible ataque(largo mayor a cinco)
		char* clave_actual = hash_iter_ver_actual (hash_iter);
		lista_t* lista_de_tiempos_actual = hash_obtener(hash, clave_actual);
		if (lista_largo(lista_de_tiempos_actual) >= 5){
			lista_iter_t* iter_primero = lista_iter_crear(lista_de_tiempos_actual);
			if(!iter_primero) return NULL;
			lista_iter_t* iter_quinto = lista_iter_crear(lista_de_tiempos_actual);
			if(!iter_quinto) return NULL;
			for(int i = 0; i< CANT_ITER ; i++) lista_iter_avanzar(iter_quinto);
			while(!lista_iter_al_final(iter_quinto)){ 	
				char* tiempo1 = lista_iter_ver_actual(iter_primero);
				char* tiempo5 = lista_iter_ver_actual(iter_quinto);
				double diferencia = calcular_diferencia(tiempo1, tiempo5);
				if (diferencia< CANT_DIF && diferencia> -CANT_DIF) {
					heap_encolar(heap_ataque, clave_actual);
					break;
				}
				lista_iter_avanzar(iter_quinto);
				lista_iter_avanzar(iter_primero);
				free(tiempo1);
				lista_borrar_primero(lista_de_tiempos_actual);
			}
			lista_iter_destruir(iter_primero);
			lista_iter_destruir(iter_quinto);
		}	
		lista_destruir(lista_de_tiempos_actual, free);
	}
	hash_iter_destruir(hash_iter);
	return heap_ataque;
}
		
bool leer_archivo(FILE *archivo, abb_t* arbol) {
	hash_t* hash = hash_crear(NULL);
	if (!hash) return false;
	char * linea = NULL;
	size_t cant = 0;
	ssize_t leidos;
	while ((leidos = getline (&linea, &cant, archivo) > 0)){
		registro_t* registro = leer_linea(linea, cant, archivo);
		if (!registro) return false;
		agregar_hash_ataque(hash, registro);
		abb_guardar(arbol, registro->ip, NULL); // Se agrega el archivo al guardar los IPs en un ABB
		registro_destruir(registro);
		}

	heap_t* ataques = detectar_ataque(hash);
	if (!ataques) return false;
	while (!heap_esta_vacio(ataques)){
		char* ip = heap_desencolar(ataques);
		fprintf (stdout, "DoS: %s\n", ip);
		}	
	heap_destruir(ataques,free);
	hash_destruir(hash);
	free(linea);
		return true;
}	

bool agregar_archivo(char** arch, abb_t* arbol) {
	FILE* archivo = fopen(arch[1], "r");
	if (!archivo)return false;
	leer_archivo(archivo, arbol); 

	fclose(archivo);
	return true;
}

// VER VISITANTES

bool imprimir_ver_visitantes( char** comando , abb_t* abb){
	fprintf(stdout,"Visitantes:\n");
	abb_in_order(abb, comando [1], comando [2], visitar_ip, NULL); 
	// El iterador interno fue modificado con el objetivo de iterar por 
	// el arbol perodesde un inicio a un final que le paso por parametro.
	return true;
}

//ORDENAR ARCHIVOS

int comparar_registros(const void* elemento1, const void* elemento2) {
	registro_t* registro_1 = (registro_t*)elemento1;
	registro_t* registro_2 = (registro_t*)elemento2;	
	
	int comparacion = strcmp(registro_2->fecha, registro_1->fecha);

	if (comparacion == 0) {
		comparacion = comparar_ips (registro_1->ip,registro_2->ip);
		if (comparacion == 0) comparacion = strcmp(registro_2->recurso, registro_1->recurso);
	}

	return comparacion;
}

aux_t* crear_estructura_auxiliar (registro_t* registro, FILE* archivo, int cantidad){
	aux_t* aux = malloc(sizeof(aux_t));
	if (!aux) return NULL;	
	aux->datos = registro;
	aux->archivo = archivo;
	aux->particion = cantidad;
	return aux;
}

int comparar_estructura_auxiliar(const void* a1, const void* a2) {
	const aux_t* aux1 = a1;
	const aux_t* aux2 = a2;
	return comparar_registros(aux1->datos, aux2->datos);
}
 
int replacementSelection(FILE* archivo, size_t k, const char* nombre_archivo){
	char* linea = NULL;
	size_t cantidad = 0;
	ssize_t leidos;  
	heap_t* heap = heap_crear(comparar_registros);
	if (!heap) return 0;

	for(int i = 0 ; i< k ; i++){   // Se encolan los primero K elementos en un heap de minimos
		if ((leidos = getline (&linea, &cantidad, archivo)) <= 0) break;
		registro_t* registro = leer_linea(linea, cantidad, archivo);
		heap_encolar(heap, registro);
	}

	lista_t* lista = lista_crear();
	if (!lista) return 0;
	int numero_particion = 1;
	bool continuo_particiones = true;
	while(continuo_particiones){
		while (!lista_esta_vacia(lista)) heap_encolar(heap, lista_borrar_primero(lista));
		
		if (leidos <= 0) break;
		FILE* archivo_particion = crear_archivo_particion(nombre_archivo, numero_particion, "w");
		
		if (!archivo_particion) return destruir_todo_ordenar(nombre_archivo, numero_particion, heap, lista, linea);
		while (!heap_esta_vacio(heap)){
			registro_t* desencolado = heap_desencolar(heap);
			fprintf(archivo_particion, "%s\n", desencolado->linea);
			
			if ((leidos = getline(&linea, &cantidad, archivo)>0)){
			// Se lee una nueva linea y se encola o inserta en la lista dependendiendo del último insertado en la partición
				registro_t* registro = leer_linea(linea,cantidad,archivo);
				if (comparar_registros(desencolado, registro) >0 ) heap_encolar(heap,registro);
				else lista_insertar_primero (lista, registro);
			} 
			registro_destruir(desencolado);
		}    
		fclose(archivo_particion);
		numero_particion++;
	}

	if (!heap_esta_vacio(heap)){ // Podría no tener más lineas por leer pero tener registros por insertar
		FILE* archivo_particion = crear_archivo_particion(nombre_archivo, numero_particion, "w");
		if (!archivo_particion) return destruir_todo_ordenar (nombre_archivo, numero_particion, heap, lista, linea);
		while (!heap_esta_vacio(heap)){
			registro_t* desencolado = heap_desencolar(heap);
			fprintf(archivo_particion, "%s\n", desencolado->linea);
			registro_destruir(desencolado);
		}
		fclose(archivo_particion);
	}
	heap_destruir(heap, free);
	lista_destruir(lista, free);
	free(linea);
	return numero_particion;
}  
 
bool juntark (const char* archivo_entrada, int cantidad_archivos, FILE* archivo_salida){
 
	heap_t* heap = heap_crear(comparar_estructura_auxiliar);
	if (!heap) return false;
	char* linea = NULL;
	size_t cantidad = 0;
	ssize_t leidos;
	for (int numero_archivo = 1; numero_archivo< (cantidad_archivos+1); numero_archivo++){
	// Se encola en un heap de mínimos la primer linea de cada archivo_particion
		FILE* archivo_particion= crear_archivo_particion(archivo_entrada, numero_archivo, "r");
		if ((leidos = getline(&linea,&cantidad,archivo_particion)) <0 ) break;
		registro_t* registro = leer_linea (linea, cantidad, archivo_particion);
		aux_t* aux = crear_estructura_auxiliar(registro, archivo_particion, numero_archivo);
		if (!aux) return destruir_todo_ordenar(archivo_entrada, cantidad_archivos, heap, NULL, linea);
		heap_encolar(heap, aux);
	} 
	
	while(!heap_esta_vacio(heap)){
		aux_t* tope = heap_desencolar(heap);
		fprintf(archivo_salida, "%s\n", tope->datos->linea); // Se imprime en el archivo final
		registro_destruir(tope->datos);
		
	   // Se busca el archivo al cual pertenece el último desencolado y se lee del mismo archvivo
		if((leidos = getline(&linea,&cantidad,tope->archivo) > 0)){
			registro_t* registro = leer_linea(linea, cantidad, tope->archivo);           
			aux_t* aux = crear_estructura_auxiliar(registro, tope->archivo, tope->particion);
			if (!aux) return destruir_todo_ordenar(archivo_entrada, cantidad_archivos, heap, NULL, linea);
			heap_encolar(heap, aux);
		}else {
			fclose(tope->archivo);
			char* nombre = crear_nombre_particion(archivo_entrada, tope->particion);
			remove(nombre); // Al finalizar de leer el archivo_particion se elimina
			free(nombre);
		}
		free(tope);
	}
	heap_destruir(heap, free);
	free(linea);
	return true;
}

size_t cantidad_particiones(size_t memoria){
	size_t memk = memoria * KILO;
	return memk/TAMANIO_LINEA;
}

bool ordenar_archivo(char** comando, size_t memoria){

	size_t lineas_part = cantidad_particiones (memoria);
	FILE* archivo_entrada = fopen (comando[1], "r");
	if (!archivo_entrada) return false;
	FILE* archivo_salida = fopen (comando[2], "w");
	if (!archivo_salida) return false;

	int num_archivos = replacementSelection(archivo_entrada, lineas_part, comando[1]);
	bool ordenado = juntark(comando[1], num_archivos, archivo_salida);
	
	fclose(archivo_entrada);
	fclose(archivo_salida);
	return ordenado;
}
