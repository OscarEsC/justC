/*
   Practica 3 programa 4: numero par (nivel de bits)
   Autor: Espinosa Curiel Oscar
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int strIsDigit(char str[]){
   /*
      Funcion que valida si una cadena es un numero entero positivo.
      Devuelve 1 si lo es, 0 en caso contrario.
   */
   int j = 0;
   int isDigit;

   while(j < strlen(str)){
      isDigit = isdigit(str[j]);
      if (isDigit == 0) return 0;
      j++;
   }

   return 1;
}

int getNumber(char n_char[]){
   /*
      Funcion que obtiene el int de un string.
      Valida que no tenga mas de 3 digitos y que
      sea precisamente int.
      Devuelve -2 si tiene mas de 3 digitos,
      -1 si no es un numero totalmente
      el numero casteado si cumple las reglas.
   */
   int n;
   if (strlen(n_char) > 3){
      //printf("Maximo un numero de 3 digitos\n");
      return -2;
   }
   if(strIsDigit(n_char)) {
      n = atoi(n_char);
      return n;
   }
   else 
      return -1;
}

int main(int argc, char *argv[])
{
	int n;
    if (argc != 2) {
        printf("\nDa (solo) un argumento al eiecutable\n");
        return -1;
    }
    //obtenemos el numero del argumento
    n = getNumber(argv[1]);
    //Operador ternario para imprimir segun el resultado
    //Hacemos AND con el bit menos significativo de n. Si da 1
    //es par, si da 0 es impar
    printf((n & 1)?"%d es impar\n":"%d es par\n", n);

    return 0;
}