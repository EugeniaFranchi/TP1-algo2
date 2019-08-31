#ifndef USUARIO_H
#define USUARIO_H

#include <stdlib.h>
#include "lista.h"

typedef struct usuario {
	unsigned int id;
	char* coordenadas;
	double saldo;
} usuario_t;

//Crea un usuario y lo devuelve.
//Post: el usuario fue creado
usuario_t* usuario_crear(char* id, char* coordenadas, char* saldo);

//Destruye el usuario.
//Pre: el usuario fue creado.
void usuario_destruir(usuario_t* usuario);

usuario_t* usuario_buscar(lista_t* usuarios, size_t id);
#endif
