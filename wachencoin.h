#ifndef WACHENCOIN_H
#define WACHENCOIN_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "lista.h"
#include "pila.h"
#include "pago.h"
#include "usuario.h"

/* ************************************************************
 *                   PRIMITIVAS PROGRAMA
 * ***********************************************************/ 
//Agrega a la cola de procesamiento la transacción.
void agregar_pago (char** parametros, lista_t* pagos, lista_t* proceso);

//Guarda en un archivo predeterminado la cantidad y el monto total
//de los pagos sin procesar.
void pagos_pendientes(char** parametros, lista_t* pagos, lista_t* proceso);

//Procesa hasta un número no negativo de pagos pendientes
void procesar (char** parametros, lista_t* pagos, lista_t* usuarios, lista_t* proceso);

//Guarda en un archivo el estado actual de las cuentas.
//Si no existe lo crea, y si existe lo sobreescribe.
void guardar_cuentas (char** parametros, lista_t* usuarios, lista_t* proceso);

#endif
