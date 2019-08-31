#define _GNU_SOURCE
#define POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "strutil.h"
#include "pila.h"
#include "pago.h"
#include "usuario.h"

#define NOMBRE_ARCH_ERRORES "errores.txt"
#define SEP_CODIGO ';'
#define SEP_PARAMETROS ' '
#define SEP_LINEA ','
#define SEP_ERROR ' '
#define VAL_USUARIO "validar_usuario"
#define VAL_PAGO "validar_pago"
#define PAGAR "pagar"
#define CANT_PARAM_AGREGAR 4
#define CANT_PARAM_PENDIENTES 1
#define CANT_PARAM_PROCESAR 2
#define CANT_PARAM_GUARDAR 2
#define LARGO_LINEA 50
#define LARGO_ERROR 50
#define CANT_USUARIOS 2
#define MAX_CARACS 300


/* ************************************************************
 *                   FUNCIONES AUXILIARES
 * ***********************************************************/ 
//Suma el monto del pago al segundo parametro.
//Post: num2 apunta a la suma de los numeros.
bool sumar_pago(void* pago, void* monto){
	double monto1 = pago_ver_monto(pago);
	double* monto2 = (double*) monto;
	*monto2 += monto1;
	return true;
}

//Devuelve la cantidad de elementos en un arreglo.
int cant_parametros(char** parametros){
	int pos = 0;
	while (parametros[pos]){
		pos++;
	}
	return pos;
}

//Recibe el nombre de un archivo y crea una lista de usuarios con sus datos.
//Devuelve NULL en caso de error.
lista_t* crear_lista_usuarios(const char* archivo){
	size_t largo = LARGO_LINEA;
	lista_t* usuarios = lista_crear();
	if(!usuarios){
		return NULL;
	}
	char* linea = NULL;
	
	FILE* arch = fopen(archivo,"r");
	size_t leido;
	while ((leido = getline(&linea, &largo, arch)) > 0 && leido != -1){
		if(linea[leido - 1] == '\n'){
			linea[leido - 1] = '\0';
		}
		char** items = split(linea, SEP_LINEA);
		usuario_t* usuario = usuario_crear(items[0], items[2], items[1]);
		if (!usuario){
			fclose(arch);
			free(linea);
			free_strv(items);
			return NULL;
		}
		lista_insertar_ultimo(usuarios, usuario);
		free(linea);
		linea = NULL;
		free_strv(items);
	}
	free(linea);
	fclose(arch);
	return usuarios;		
}

//Imprime el contenido del archivo de errores.
void imprimir_estado_final(){
	FILE* arch = fopen(NOMBRE_ARCH_ERRORES, "r");
	size_t leido;
	char* linea = NULL;
	size_t largo = LARGO_ERROR;
	while ((leido = getline(&linea, &largo, arch)) > 0 && leido != -1){
		if(linea[leido - 1] == '\n'){
			linea[leido - 1] = '\0';
		}
		char** items = split(linea, SEP_ERROR);
		if (items[1] && items[2] && strcmp(items[2],"comando") == 0){
			fprintf(stderr,"%s\n", linea);
			free_strv(items);
			break;
		}
		if (items[1] && items[2] && strcmp(items[2],"pago") == 0){
			fprintf(stderr,"%s\n", linea);
		}
		else{
			fprintf(stdout,"%s\n", linea);
		}
		free(linea);
		linea = NULL;
		free_strv(items);
	}
	free(linea);
	fclose(arch);
}

//Destruye el usuario dato.
void destruir_usuario(void* dato){
	usuario_t* usuario = (usuario_t*) dato;
	usuario_destruir(usuario);
}
 
//Destruye el pago dato.
void destruir_pago(void* dato){
	pago_t* pago = (pago_t*) dato;
	pago_destruir(pago);
}

/* ************************************************************
 *                        VALIDACIONES
 * ***********************************************************/ 
//Desapila dos elementos y devuelve true si las coordenadas son
//apropiadas para el usuario, de lo contrario false.
bool validar_usuario(pila_t* pila, lista_t* usuarios, size_t num_pago){
	char* coords = (char*)pila_desapilar(pila);
	char* id_char = (char*) pila_desapilar(pila);
	unsigned int id = atoi(id_char);
	
	usuario_t* usuario = usuario_buscar(usuarios, id);
	
	if (!usuario || strcmp(usuario->coordenadas, coords)!= 0){
		FILE* arch = fopen(NOMBRE_ARCH_ERRORES, "a");
		fprintf(arch, "Error en pago %zu\n", num_pago);
		fclose(arch);
		return false;
	}
	return true;
}

//Desapila dos elementos y devuelve true si el usuario tiene fondos
//suficientes para hacer la operación.
bool validar_pago(pila_t* pila, lista_t* usuarios, size_t num_pago){
	double monto = atof(pila_desapilar(pila));
	char* id_char = (char*) pila_desapilar(pila);
	unsigned int id = atoi(id_char);
	
	usuario_t* usuario = usuario_buscar(usuarios, id);
	if (!usuario || usuario->saldo < monto){
		FILE* arch = fopen(NOMBRE_ARCH_ERRORES, "a");
		fprintf(arch, "Error en pago %zu\n", num_pago);
		fclose(arch);
		return false;
	}
	return true;
}

//Desapila tres elementos y hace el movimiento de fondos.
//Devuelve false en caso de error.
bool pagar(pila_t* pila, lista_t* usuarios, size_t num_pago){
	char* id_char1 = (char*) pila_desapilar(pila);
	unsigned int id1 = atoi(id_char1);
	char* id_char2 = (char*) pila_desapilar(pila);
	unsigned int id2 = atoi(id_char2);
	double monto = atof(pila_desapilar(pila));
	
	usuario_t* usuario1 = usuario_buscar(usuarios, id1);
	if (!usuario1){
		FILE* arch = fopen(NOMBRE_ARCH_ERRORES, "a");
		fprintf(arch, "Error en pago %zu\n", num_pago);
		fclose(arch);
		return false;
	}
	usuario_t* usuario2 = usuario_buscar(usuarios, id2);
	if (!usuario2){
		FILE* arch = fopen(NOMBRE_ARCH_ERRORES, "a");
		fprintf(arch, "Error en pago %zu\n", num_pago);
		fclose(arch);
		return false;
	}
	
	usuario1->saldo -= monto;
	usuario2->saldo += monto;
	
	return true;
}

/* ************************************************************
 *                   PRIMITIVAS PROGRAMA
 * ***********************************************************/ 
bool agregar_pago (char** parametros, lista_t* pagos){
	if (cant_parametros(parametros) != CANT_PARAM_AGREGAR){
		FILE* arch = fopen(NOMBRE_ARCH_ERRORES, "a");
		fprintf(arch, "Error en comando agregar_pago\n");
		fclose(arch);
		return false;
	}
	pago_t* pago = pago_crear(parametros[1], parametros[2], parametros[3]);
	lista_insertar_ultimo(pagos, pago);
	
	FILE* arch = fopen(NOMBRE_ARCH_ERRORES, "a");
	fprintf(arch, "OK\n");
	fclose(arch);
	return true;
}

bool pagos_pendientes(char** parametros, lista_t* pagos){
	if (cant_parametros(parametros) != CANT_PARAM_PENDIENTES){
		FILE* arch = fopen(NOMBRE_ARCH_ERRORES, "a");
		fprintf(arch, "Error en comando pagos_pendientes\n");
		fclose(arch);
		return false;
	}
	double total = 0;
	double* suma = &total;
	lista_iterar(pagos, sumar_pago, suma);
	size_t cant =  lista_largo(pagos);
	
	FILE* arch = fopen(NOMBRE_ARCH_ERRORES, "a");
	fprintf(arch, "%zu,%.3f\n", cant, total);
	fprintf(arch, "OK\n");
	fclose(arch);
	return true;
}

bool procesar (char** parametros, lista_t* pagos, lista_t* usuarios){
	if (cant_parametros(parametros) != CANT_PARAM_PROCESAR){
		FILE* arch = fopen(NOMBRE_ARCH_ERRORES, "a");
		fprintf(arch, "Error en comando procesar\n");
		fclose(arch);
		return false;
	}
	bool validez;
	size_t numero = atoi(parametros[1]);
	for (size_t i = 0; i<numero; i++){
		pago_t* pago = borrar_pago(pagos);
		if (!pago){
			break;
		}
		pila_t* pila_aux = pila_crear();
		char* codigo = pago_ver_codigo(pago);
		char** comandos = split(codigo, SEP_CODIGO);
		size_t pos = 0;
		char* comando = comandos[pos];
		while (comando){
			if (strcmp(comando, VAL_USUARIO) == 0){
				validez = validar_usuario(pila_aux, usuarios, pago_ver_id(pago));
			}
			else if (strcmp(comando, VAL_PAGO) == 0){
				validez = validar_pago(pila_aux, usuarios, pago_ver_id(pago));
			}
			else if (strcmp(comando, PAGAR) == 0){
				validez = pagar(pila_aux, usuarios, pago_ver_id(pago));
			}
			else{
				validez = pila_apilar(pila_aux, comando);
			}
	
			if (!validez){
				break;
			}
			pos++;
			comando = comandos[pos];

		}
		pila_destruir(pila_aux);
		free_strv(comandos);
		pago_destruir(pago);
	}
	FILE* arch = fopen(NOMBRE_ARCH_ERRORES, "a");
	fprintf(arch, "OK\n");
	fclose(arch);
	
	return true;
}

bool guardar_cuentas (char** parametros, lista_t* usuarios){
	if (cant_parametros(parametros) != CANT_PARAM_GUARDAR){
		FILE* arch = fopen(NOMBRE_ARCH_ERRORES, "a");
		fprintf(arch, "Error en comando guardar_cuentas\n");
		fclose(arch);
		return false;
	}
	const char* nombre_arch = parametros[1];
	FILE* arch_cuentas = fopen(nombre_arch, "w");
	lista_iter_t* iter = lista_iter_crear(usuarios);
	for (;!lista_iter_al_final(iter); lista_iter_avanzar(iter)){
		usuario_t* usuario = (usuario_t*)lista_iter_ver_actual(iter);
		fprintf(arch_cuentas, "%i,%.3f,%s\n", usuario->id, usuario->saldo, usuario->coordenadas);
	}
	fclose(arch_cuentas);
	lista_iter_destruir(iter);
	
	FILE* arch = fopen(NOMBRE_ARCH_ERRORES, "a");
	fprintf(arch, "OK\n");
	fclose(arch);
	return true;
}

int main(int argc, char **argv){
	if (argc != 2){
		return 1;
	}
	const char* archivo = argv[1];
	lista_t* usuarios = crear_lista_usuarios(archivo);
	lista_t* pagos = lista_crear();
	char* linea = NULL;
	size_t leido;
	size_t largo = MAX_CARACS;
	
	while ((leido = getline(&linea, &largo, stdin)) > 0 && leido != -1){
		if(linea[leido - 1] == '\n'){
			linea[leido - 1] = '\0';
		}
		char** parametros = split(linea, SEP_PARAMETROS);
		if (strcmp(parametros[0], "agregar_pago") == 0){
			agregar_pago(parametros, pagos);
		}
		else if (strcmp(parametros[0], "pagos_pendientes") == 0){
			pagos_pendientes(parametros, pagos);
		}
		else if (strcmp(parametros[0], "procesar") == 0){
			procesar(parametros, pagos, usuarios);
		}
		else if (strcmp(parametros[0], "guardar_cuentas") == 0){
			guardar_cuentas(parametros, usuarios);
		}
		else if (strcmp(linea, "finalizar") == 0){
			free(linea);
			linea = NULL;
			free_strv(parametros);
			break;
		}
		else{
			FILE* arch = fopen(NOMBRE_ARCH_ERRORES, "a");
			fprintf(arch, "Error en comando %s\n", parametros[0]);
			fclose(arch);
		}
		free(linea);
		linea = NULL;
		free_strv(parametros);
	}
	FILE* arch = fopen(NOMBRE_ARCH_ERRORES, "a");
	fprintf(arch, "OK\n");
	fclose(arch);
	lista_destruir(usuarios, destruir_usuario);
	lista_destruir(pagos, destruir_pago);
	imprimir_estado_final();
	remove(NOMBRE_ARCH_ERRORES);
	return 0;
}
