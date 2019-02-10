/*
	Practica 8: Ultimo caracter del ultimo argumento
	Autor: Espinosa Curiel Oscar
*/
#include <stdio.h>
#include <string.h>
int main(int argc, char ** argv){
	printf("%c\n", *(*(argv + argc - 1) + strlen(*(argv + argc - 1)) - 1));
	return 0;
}