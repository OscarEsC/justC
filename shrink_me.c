//Practica 4
//"Hackea" este programa
#include<stdio.h>
#include<limits.h>
int main()
{
	unsigned int var1,var2,mayor;
	printf("Ingresa dos enteros sin signo(maximo %u):",UINT_MAX);
	scanf("%d",&var1);
	scanf("%d",&var2);
	if(var1>var2)
		mayor=var1;
	else if (var2>var1)
		mayor=var2;
	printf("El mayor es %d\n",mayor);
}
