#include <stdlib.h>
#include "lista.h"
#include "pago.h"
#include <string.h>

struct pago {
	size_t id;
	char* codigo;
	double monto;
};

/* ************************************************************
 *                     PRIMITIVAS PAGO
 * ***********************************************************/ 
pago_t* pago_crear(char* id_pago, char* monto, char* codigo){
	pago_t* pago = malloc(sizeof(pago_t));
	if (!pago){
		return NULL;
	}
	char* id_pago_char = id_pago ;
	char* monto_char = monto;
	char* codigo_dup = malloc(sizeof(char) * strlen(codigo)+1);
	if(!codigo_dup){
		free(pago);
		return NULL;
	}
	strcpy(codigo_dup, codigo);
	
	size_t id_pago_size = atoi(id_pago_char);
	double monto_double = atof(monto_char);
	
	pago->id = id_pago_size;
	pago->codigo = codigo_dup;
	pago->monto = monto_double;
	
	return pago;
}

pago_t* borrar_pago(lista_t* pagos){
	return lista_borrar_primero(pagos);
}

void pago_destruir(pago_t* pago){
	free(pago->codigo);
	free(pago);
}

char* pago_ver_codigo(pago_t* pago){
	return pago->codigo;
}

double pago_ver_monto(pago_t* pago){
	return pago->monto;
}

size_t pago_ver_id(pago_t* pago){
	return pago->id;
}
