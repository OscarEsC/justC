/*
   Practica 3 programa 5: numeros de Fibonacci recursivo
   Autor: Espinosa Curiel Oscar
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int strIsDigit(char str[]){
  /*
    Funcion que valida si una cadena es un numero
    entero positivo.
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
      Devuelve -1 si no es un numero totalmente o
      el numero casteado si cumple las reglas.
   */
   int n;
   if(strIsDigit(n_char)) {
      n = atoi(n_char);
      return n;
   }
   else 
      return -1;
}

void fibonacci(int n1, int n2, int count) {
    /*
      Funcion que calcula de manera recursiva los numeros
      de Fibonacci.
      En la condicion de paro, se imprimen los ultimos 2 numeros.
      La condicion de paro se cumple cuando se han calculano n
      numeros de Fibonacci.
    */
    if (count == 0) {
      printf("%d\n%d\n", n1, n2);
      
    }
    else {
      printf("%d\n", n1);
      fibonacci(n2, n1 + n2, count - 1);
    }
}

int main(int argc, char *argv[])
{
    int n1, n2, count;
    if (argc != 4) {
        printf("\nDa (solo) tres argumentos al ejecutable\n");
        return -1;
    }
    n1 = getNumber(argv[1]);
    n2 = getNumber(argv[2]);
    count = getNumber(argv[3]);
    //Se valida el valor obtenido en getNumber de cada argumento
    if (n1 > 0 && n2 > 0 && count > 0)
      fibonacci(n1, n2, count);
    else
      printf("Argumentos no validos.\n");

    return 0;
}