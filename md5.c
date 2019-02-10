/*
	Practica 12: MD5
	Autor: Espinosa Curiel Oscar
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>

int str2md5(char *str, char *out){
	/*
		Funcion que obtiene el hash md5 hex de la cadena recibida
		como primer parametro y la copia en el segundo parametro.
		El segundo parametro debe ser de 2*strlen(str) + 1
	*/
	int i;
	char digest[16];
	//Obtenemos el hash
	MD5(str,(size_t) strlen(str), digest);
	//iteramos sobre el hash para convertirlo a HEX
	for(i = 0; i < strlen(digest); i++){
		snprintf(&(out[i*2]), 16*2, "%02x", (unsigned int)digest[i]);
	}
	return 0;
}

int main(int argc, char  **argv){
	char digest[33];
	str2md5(*(argv + 1), digest);
	printf("%s\n", digest);
	return 0;
}