/*
	Practica 2: Tiempo Unix maximo posible almacenado
	Autor: Espinosa Curiel Oscar
*/

//En esta biblioteca esta definida INT_MAX
#include <limits.h>
#include <stdio.h>
//Esta biblioteca se usa para los time
#include <time.h>

int main(){
	//Obtenemos el tiempo Unix actual
	time_t current_date = time(NULL);
	//Obtenemos el tiempo Unix maximo que se puede almacenar
	time_t last_day = (time_t)INT_MAX;
	//localtime nos da el formato en la hora local
	printf("Fecha local actual: \n%s",asctime(localtime(&current_date)));
	printf("\nFecha local en que acaba el espacio para la fecha Unix:");
	printf("\n%s\n", asctime(localtime(&last_day)));
	return 0;
}