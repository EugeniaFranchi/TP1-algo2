#include <stdlib.h>
#include "usuario.h"
#include <string.h>

/* ************************************************************
 *                    PRIMITIVAS USUARIO
 * ***********************************************************/ 
usuario_t* usuario_crear(char* id, char* coordenadas, char* saldo){
	usuario_t* usuario = malloc(sizeof(usuario_t));
	if (!usuario){
		return NULL;
	}

	char* id_char = (char*) id;
	char* saldo_char = (char*) saldo;
	char* coords_dup = malloc(sizeof(char) * strlen(coordenadas)+1);
	if(!coordenadas){
		free(usuario);
		return NULL;
	}
	strcpy(coords_dup, coordenadas);

	unsigned int id_int = atoi(id_char);
	double saldo_double = atof(saldo_char);
	 
	usuario->id = id_int;
	usuario->coordenadas = coords_dup;
	usuario->saldo = saldo_double;
	
	return usuario;
}

void usuario_destruir(usuario_t* usuario){
	free(usuario->coordenadas);
	free(usuario);
}

usuario_t* usuario_buscar(lista_t* usuarios, size_t id){
	lista_iter_t* iter = lista_iter_crear(usuarios);
	for (; !lista_iter_al_final(iter); lista_iter_avanzar(iter)){
		usuario_t* usuario = (usuario_t*)lista_iter_ver_actual(iter);
		if (usuario->id == id){
			void* dato = lista_iter_ver_actual(iter);
			lista_iter_destruir(iter);
			return dato;
		}
	}
	lista_iter_destruir(iter);
	return NULL;
}
