/*
	Practica 3 programa 1: comparador de contrasenas
	Autor: Espinosa Curiel Oscar
*/
#include <stdio.h>
#include <stdlib.h>
//Biblioteca para el manejo de cadenas (strcpy, strcmp)
#include <string.h>

int checkPasswd (char passwd[], int attempts) {
	/*
		Funcion que compara dos cadenas con n numero de intentos
		permitidos de errar al ingresar la contrasena
	*/
	int current_attempts = 0;
	char attempt[30];
	while(current_attempts < attempts) {
		printf("\nEscribe la contrasena:  ");
		scanf("%s", &attempt);
		
		//Si las cadenas son iguales, se termina la ejecucion	
		if (strcmp(passwd, attempt) == 0) {
			printf("\nContrasena correcta.\n");
			return 1;
		}

		printf("\nLa contrasena no coincide.");

		attempts++;
	}

	printf("\nSe ha alcanzado el limite de oportunidades.\n");
	return 0;
}
int main(int argc, char *argv[]){
	char passwd[30];
	int attempts = 3;
	//La contrasena a validar se da como argumento al ejecutar el programa
	if (argc != 2){
		printf("Ingresa (solo) una contrasena maestra\ncomo argumento del ejecutable.\n");
		return 1;
	}

	strcpy(passwd, argv[1]);
	//Comparamos las contrasenas con 3 intentos permitidos
	checkPasswd(passwd, 3);
	return 0;
}