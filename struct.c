/*
	Practica : Estructuras en C
	Autor: Espinosa Curiel Oscar
*/

#include <stdio.h>
#include <stdlib.h>

//Definicion de la estructura instructor
typedef struct instructor {
	char *nombre;
	char *curso;
	int edad;
	int *carisma;
}instructor;


instructor inst(instructor i){
	/*
		Funcion que retorna una estructura instructor
		recibe una estructura instructor
	*/
	i.nombre = "Fernando";
	i.curso[0] = 'c';
	i.edad = 28;
	i.carisma = NULL;

	return i;
}

int main(){
	//Declaracion y reserva de memoria de la estructura
	instructor i;
	i.nombre = (char *)malloc(20*sizeof(char));
	i.curso = (char *)malloc(2*sizeof(char));
	i = inst(i);
	printf("%s\n", i.nombre);
	printf("%s\n", i.curso);
	printf("%d\n", i.edad);
	printf("%d\n", i.carisma);
	return 0;
}