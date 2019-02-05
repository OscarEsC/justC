/*
	Practica 1: FizzBuzz
	Autor: Espinosa Curiel Oscar
*/
#include <stdio.h>

int main(){
	int i;
	for(i = 30; i > 0; i--)
		//Utilizo operadores tenarios anidados para la impresion 
		//correspondiente
		printf((i%15==0)?"FizzBuzz\n":(i%5==0)?"Buzz\n":(i%3==0)?"Fizz\n":"%d\n",i);
	return 0;
}