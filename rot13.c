/*
   Practica 3 programa 3: algoritmo ROT13
   Autor: Espinosa Curiel Oscar
*/
#include<stdio.h>
#include <string.h>

#define ROT 13
#define sizeC 30
int rotStr(char str[], char rot[]){
    /*
        Funcion que obtiene el algoritmo rot13
        de la primer cadena dada como parametro.
        El resultado lo escribe en la segunda
        cadena dada como parametro.
        Hace un corrimiento de 13 letras de a-z y
        A-Z. Cualquier otro caracter lo copia tal cual
    */
    int i = 0;
    char c;
    while (i < strlen(str)){
        if (str[i] >= 'A' && str[i] <= 'Z'){
            if ((c = str[i] + ROT) <= 'Z') {
                rot[i] = c;
            }
            else {
                rot[i] = str[i] - ROT;
            }
        }
        else if (str[i] >= 'a' && str[i] <= 'z'){
            if ((c = str[i] + ROT) <= 'z') {
                rot[i] = c;
            }
            else {
                rot[i] = str[i] - ROT;
            }
        }
        else
            rot[i] = str[i];

        i++;
    }
    //printf("%s\n", rot);
    return 0;
}

int main(int argc, char *argv[])
{
    char str[sizeC], rot[sizeC];
    if (argc != 2) {
        printf("\nDa (solo) un argumento del eiecutable\n");
        return -1;
    }
    strcpy(str,argv[1]);
    rotStr(str,rot);
    printf("%s\n", rot);
    //strcpy(rot,str);
    return 0;
}