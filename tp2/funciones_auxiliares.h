#ifndef FUNC_AUX_H
#define FUNC_AUX_H
#include <stdbool.h>
#include "strutil.h"
#include "heap.h"
#include "lista.h"

struct registro;

int comparar_ips(const void* ip1, const void* ip2);
struct registro* leer_linea (char* linea, size_t cantidad, FILE*archivo);
char* crear_nombre_particion(const char* nombre, size_t particion) ;
FILE* crear_archivo_particion(const char* nombre, size_t particion, char* modo);
int destruir_todo_ordenar (const char* nombre, size_t particion, heap_t* heap, lista_t* lista, char* linea);
int calcular_diferencia (char* tiempo1, char* tiempo5);
char* crear_copia (char* fecha);
bool visitar_ip (const char* clave, void* dato, void* extra);


#endif // FUNC_AUX
