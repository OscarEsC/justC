//Practica 4: 
//Autor: Espinosa Curiel Oscar
//"Hackea" este programa
#include<stdio.h>
#include<limits.h>
int main()
{
	unsigned int var1,var2;
	printf("Ingresa dos enteros sin signo(maximo %u):",UINT_MAX);
	scanf("%d %d",&var1, &var2);
	printf("El mayor es %d\n",(var1>var2)?var1:var2);
}