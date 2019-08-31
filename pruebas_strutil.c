#include "strutil.h"
#include <stdio.h>
#include "testing.h"
#include <stdlib.h>
#include <string.h>

void pruebas_crear_destruir(){
	const char* vacio = "";
	char sep = ',';
	
	char** palabras = split(vacio,sep);
	print_test("Se crea el arreglo", palabras);
	free_strv(palabras);
	print_test("Se destruye el arreglo", true);
}

void pruebas_vacia_separador(){
	const char* vacio1 = "";
	const char* vacio2 = ",";
	const char* vacio3 = ",,";
	char sep = ',';
	
	/*Pruebas sin separador*/
	char** palabras = split(vacio1,sep);
	print_test("Split de una cadena vacia", (strcmp(palabras[0],vacio1) == 0) && (palabras[1] == NULL));
	free_strv(palabras);
	
	/*Pruebas con un separador*/
	palabras = split(vacio2,sep);
	print_test("Split de una cadena vacia con un separador", (strcmp(palabras[0],vacio1) == 0) && (strcmp(palabras[1],vacio1) == 0) && (palabras[2] == NULL));
	free_strv(palabras);
	
	/*Pruebas con mas de un separador*/
	palabras = split(vacio3,sep);
	print_test("Split de una cadena vacia con un separador", (strcmp(palabras[0],vacio1) == 0) && (strcmp(palabras[1],vacio1) == 0) && (strcmp(palabras[2],vacio1) == 0) && (palabras[3] == NULL));
	free_strv(palabras);
}

void pruebas_con_cadenas(){
	const char* cadena1 = "hola";
	const char* cadena2 = "hola-mundo";
	const char* cadena3 = "hola-mundo-chau";
	char sep = '-';
	char* rta1 = "mundo";
	char* rta2 = "chau";
	
	/*Pruebas con una cadena*/
	char** palabras = split(cadena1,sep);
	print_test("Split de una cadena no separada", (strcmp(palabras[0],cadena1) == 0) && (palabras[1] == NULL));
	free_strv(palabras);
	
	/*Pruebas con un separador*/
	palabras = split(cadena2,sep);
	print_test("Split de dos cadenas separadas", (strcmp(palabras[0],cadena1) == 0) && (strcmp(palabras[1],rta1) == 0) && (palabras[2] == NULL));
	free_strv(palabras);
	
	/*Pruebas con mas de un separador*/
	palabras = split(cadena3,sep);
	print_test("Split de tres cadenas separadas", (strcmp(palabras[0],cadena1) == 0) && (strcmp(palabras[1],rta1) == 0) && (strcmp(palabras[2],rta2) == 0) && (palabras[3] == NULL));
	free_strv(palabras);
}

void pruebas_mixta(){
	const char* cadena1 = "hola/";
	const char* cadena2 = "/hola/mundo";
	const char* cadena3 = "hola//mundo";
	char sep = '/';
	char* vacio = "";
	char* rta1 = "mundo";
	char* rta2 = "hola";
	
	/*Pruebas con una cadena*/
	char** palabras = split(cadena1,sep);
	print_test("Split de una cadena separada de vacio", (strcmp(palabras[0],rta2) == 0) && (strcmp(palabras[1],vacio) == 0) && (palabras[2] == NULL));
	free_strv(palabras);
	
	/*Pruebas con un separador*/
	palabras = split(cadena2,sep);
	print_test("Split de vacio separado de cadena", (strcmp(palabras[0],vacio) == 0) && (strcmp(palabras[1],rta2) == 0) && (strcmp(palabras[2],rta1) == 0) && (palabras[3] == NULL));
	free_strv(palabras);
	
	/*Pruebas con mas de un separador*/
	palabras = split(cadena3,sep);
	print_test("Split de dos cadenas separadas con un vacio en el medio", (strcmp(palabras[0],rta2) == 0) && (strcmp(palabras[1],vacio) == 0) && (strcmp(palabras[2],rta1) == 0) && (palabras[3] == NULL));
	free_strv(palabras);
}

int main(){
	pruebas_crear_destruir();
	pruebas_vacia_separador();
	pruebas_con_cadenas();
	pruebas_mixta();
	return 0;
}
