#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//Devuelve la cantidad en str separadas por un separador sep.
size_t cant_cadenas(const char* str, char sep){
	size_t cont = 1;
	size_t pos = 0;
	while (str[pos]){
		if (str[pos] == sep){
			cont++;
		}
		pos++;
	}
	return cont;
}

//Devuelve la cantidad de caracteres desde la posicion inicio de la cadena str
//hasta el separador sep.
size_t cant_caracteres(const char* str, char sep, size_t inicio){
	size_t cont = 0;
	for (size_t pos = inicio;str[pos]; cont++, pos++){
		if (str[pos] == sep){
			break;
		}
	}
	return cont;
}

//Completa substr con tantos caracteres como largo indique, a partir de la
//posicion inicio de str.
void subdividir_str(const char* str, size_t inicio, char* substr, size_t largo){
	for (int i = 0; i < largo; i++, inicio++){
		substr[i] = str[inicio];
	}
	substr[largo] = '\0';
}

char** split(const char* str, char sep){
	size_t largo_cadenas = cant_cadenas(str, sep);
	char** cadenas = malloc(sizeof(char*) * (largo_cadenas + 1));
	if (!cadenas || sep == '\0'){
		return NULL;
	}
	cadenas[largo_cadenas] = NULL;
	
	if (strcmp(str, "") == 0){
		char* substr = malloc(sizeof(char));
		if(!substr){
			free(cadenas);
			return NULL;
		}
		substr[0] = '\0';
		cadenas[0] = substr;
	}
	
	size_t pos_str = 0;
	size_t pos_cadenas = 0;
	bool primero = true;
	
	while (str[pos_str]){
		if (!primero){
			pos_str++;
		}
		else{
			primero = false;
		}
		size_t largo = cant_caracteres(str, sep, pos_str);
		char* substr = malloc(sizeof(char) * (largo + 1));
		if(!substr){
			for (int i = 0; i<pos_cadenas; i++){
				free(cadenas[i]);
			} 
			free(cadenas);
			return NULL;
		}
		subdividir_str(str, pos_str, substr, largo);
		cadenas[pos_cadenas] = substr;
		pos_cadenas++;
		pos_str += largo;
	}
	return cadenas;
}

void free_strv(char* strv[]){
	for(int pos = 0; strv[pos]; pos++){
		free(strv[pos]);
	}
	free(strv);
}

