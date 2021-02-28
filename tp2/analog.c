#include <stdlib.h>
#include <stdio.h>
#include "comando.h"
#include "funciones_auxiliares.h"

int comparar_ips_rev(const void* ip1, const void* ip2){
	return comparar_ips(ip2,ip1);
}
 
bool validar_parametros(char** cadenas, size_t cant) {
	int i = 0;
	char* actual = cadenas[0];
	while (actual) {
		i ++;
		actual = cadenas[i];
	}
	return i == cant;
}

int mensaje_error(char * comando) {
	fprintf(stderr, "Error en comando %s\n", comando);
	return 0;
}


int main (int argc, char * argv []){
	char* linea = NULL;
	size_t cant = 0;
	ssize_t leidos;
	abb_t* arbol = abb_crear(comparar_ips_rev, NULL);
	if (!arbol) return 0;
	while((leidos = getline (&linea, &cant, stdin) > 0 )){
		linea[strcspn(linea, "\n")] = '\0';
		size_t memoria = atoi (argv[1]);
		char** comando = split(linea, ' ');
        	bool ok;
        	if (strcmp (comando[0], "ordenar_archivo" ) == 0){
        		if (!validar_parametros(comando, 3)) ok = false;
			else ok = ordenar_archivo(comando,memoria);
		}
		else if (strcmp (comando[0], "agregar_archivo" ) == 0){
			if (!validar_parametros(comando, 2)) ok = false;
        		else ok = agregar_archivo(comando, arbol);
        	}
        	else if (strcmp (comando[0], "ver_visitantes") == 0){
            		if (!validar_parametros(comando, 3)) ok = false;
           		else ok = (imprimir_ver_visitantes (comando, arbol));
        	}
        	if (ok) fprintf(stdout, "OK\n");
		else {
			mensaje_error(comando[0]);
			free_strv(comando);
			break;
		}
		free_strv(comando);
    	}
    	abb_destruir(arbol);
    	free(linea);
    	return 0;
}


