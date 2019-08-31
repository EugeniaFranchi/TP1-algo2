#ifndef PAGO_H
#define PAGO_H

#include <stdlib.h>
#include "lista.h"

typedef struct pago pago_t;

//Inicializa el pago.
//Post: el pago fue creado.
pago_t* pago_crear(char* id_pago, char* monto, char* codigo);

//Borra el primer pago de la lista y lo devuelve.
//En caso de error devuelve NULL.
pago_t* borrar_pago(lista_t* pagos);

//Destruye el pago.
//Pre: el pago fue creado.
void pago_destruir(pago_t* pago);

//Devuelve el codigo de pago.
//El pago fue creado.
char* pago_ver_codigo(pago_t* pago);

//Devuelve el monto de pago.
//El pago fue creado.
double pago_ver_monto(pago_t* pago);

//Devuelve el id de pago.
//El pago fue creado.
size_t pago_ver_id(pago_t* pago);

#endif
