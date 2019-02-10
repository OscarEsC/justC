/*
	Practica : BubbleSort con apuntadores
	Autor: Espinosa Curiel Oscar
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int getArray(char *arrc, float *arri){
	/*
		Funcion que castea un arreglo char, mandado como primer parametro
		a un arreglo float. El arreglo final lo almacena en el apuntador 
		mandado como segundo parametro.
		Si se lee un caracter no float, devuelve un -2,
		si todos los elementos son float, devuelve un 1.
	*/
	//ptr es el apuntador que recorre cada elemento, delimitados por ','
	//aux es un apuntador que se utiliza en strtof para saber cuando se ha
	//leido un caracter que no es un  numero
	char *ptr = strtok(arrc, ","), *aux = NULL;
	float a;
	int count = 1;
	//Se redimenciona el apuntador
	arri = (float *) realloc(arri, 5*sizeof(float));
	while(ptr != NULL)
	{
		//Cada 5 elementos almacenados, redimencionamos el apuntador con otros
		//5 espacios libres
		if (count %5 == 0)
			arri = (float *) realloc(arri, (count + 5)*sizeof(float));
		
		a = strtof(ptr, &aux);
		//Cuando aux es != NULL, quiere decir que hubo un elemento que no es float
		if (*aux != NULL){
			return -2;
		}
		*(arri + count - 1) = a;
		count ++;
		ptr = strtok(NULL, ",");
	}
	return 1;
}

int bubbleSort(float *array){
	/*
		Funcion que ordena por BubbleSort un arreglo float
	*/
	int i = 0, j;
	float tmp;
	while (*(array + i)){
		j = i+1;
		while (*(array + j)){
			if (*(array + i) > *(array + j)){
				tmp = *(array + j);
				*(array + j) = *(array + i);
				*(array + i) = tmp;
			}
			j++;
		}
		i++;
	}

	return 1;
}

int printArray(float *array){
	/*
		Funcion para imprimir el arreglo ordenado
	*/
	int c = 0;
	while(*(array + c)){
		printf("%.2f\n", *(array + c));
		c++;
	}
}

int main(int argc, char **argv){
	float *array = (float *)malloc(5*sizeof(float));
	//El arreglo se manda como parametro al ejecutable
	if (argc !=2){
		printf("Parametros del ejecutable no validos\n");
		return -1;
	}
	//Si retorna 1, no hubo problemas con el array recibido como
	//parametro
	if(getArray(*(argv + 1), array) == 1){
		bubbleSort(array);
		printArray(array);
	}
	else{
		printf("No todos los datos son numeros\n");
	}
	
	return 0;
}