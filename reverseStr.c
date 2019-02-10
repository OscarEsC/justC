/*
	Practica : Cadena inversa con apuntadores
	Autor: Espinosa Curiel Oscar
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * reverseStr(char *str) {
	/*
		Funcion que revise un apuntador a una cadena. Devuelve
		un apuntador a una cadena escrita en reversa de la original.
	*/
	//Reservado de memoria dependiendo la longitud de la cadena recibida
	char *rvs = (char *)malloc(strlen(str) * sizeof(char));
	int i = 0, l = strlen(str) - 1;
	while(*(str + i)){
		*(rvs + i) = *(str + l - i);
		i++;
	}

	return rvs;
}


int main(int argc, char **argv){
	if (argc != 2){
		printf("El ejecutable necesita (solo) un argumentos\n");
		return -1;
	}
	//Reserva de memoria dependiendo la longitud del argumento
	char *rvs = (char *)malloc(strlen(*(argv + 1))* sizeof(char));
	rvs = reverseStr(*(argv + 1));
	printf("%s\n", rvs);

	return 0;
}