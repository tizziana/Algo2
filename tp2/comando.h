#ifndef TP2_H
#define TP2_H
#define _XOPEN_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>
#include "heap.h"
#include "abb.h"
#include "hash.h"
#include "lista.h"
#include "strutil.h"
#include "funciones_auxiliares.h"
#define TIME_FORMAT "%FT%T%z"


typedef struct registro registro_t;
registro_t* crear_registro(char* linea, char** datos);
int comparar_ips(const void* ip1, const void* ip2);
ssize_t getline (char* linea[], size_t * n, FILE* );
bool agregar_archivo(char** arch, abb_t* arbol);
bool imprimir_ver_visitantes( char** comando , abb_t* abb);
bool ordenar_archivo(char** comando,size_t m);


#endif
