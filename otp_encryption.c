/*
	Practica 5: cifrado One Time Pad
	Autor: Espinosa Curiel Oscar
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>


int otpEncrypt(char n_ifile[], char n_ofile[], char n_pad[]){
	/*
		Funcion que cifra el contenido de n_ifile. El mensaje cifrado
		lo escribe en n_ofile. En n_pad se escribe los caracteres con los
		que se cifro el archivo n_ifile.
		Devuelve 0 si se cifro de manera correcta, -2 si no se pudo
		abrir el archivo n_ifile
	*/
	//Semilla para obtener los caracteres pseudo-aleatoriamente
	srand(time(NULL));
	char c_read, c_random, c_write;
	//Sockets para el manejo de los archivos.
	//n_ifile debe existir, n_ofile y n_pad se sobreescriben
	FILE *ifile = fopen(n_ifile, "r"), *ofile = fopen(n_ofile, "w+");
	FILE *npad = fopen(n_pad, "w+");
	if(ifile != NULL){
		//Leemos n_ifile hasta llegar al final del archivo
		while ((c_read = fgetc(ifile)) != EOF) {
			//Generamos el caracter pseudo-aleatorio
			//Se le suma 32 para evitar los caracteres de control (codigo ASCII)
			c_random = (char)(rand()%126) + 32;
			//XOR del caracter aleatorio y el caracter del archivo a cifrar
			c_write = c_read ^ c_random;
			fputc(c_random, npad);
			fputc(c_write, ofile);
		}
		//Cierre de los sockets usados
		fclose(ifile);
		fclose(ofile);
		fclose(npad);
		return 0;
	}
	else{
		//Error si no se pudo abrir n_ifile
		return -2;
	}
}

int otpDecrypt(char n_ifile[], char n_ofile[], char n_pad[]) {
	/*
		Funcion que descifra el contenido de n_ifile. El mensaje descifrado
		lo escribe en n_ofile. De n_pad se leen los caracteres con los
		que se cifro el archivo n_ifile.
		Devuelve 0 si se descifro de manera correcta, -3 si no se pudo
		abrir el archivo n_ifile o el n_pad
	*/
	char c_read, c_random, c_write;
	//Sockets de conexion con los archivos
	//n_ifile y n_pad deben existir, n_ofile se sobreescribe
	FILE *ifile = fopen(n_ifile, "r"), *ofile = fopen(n_ofile, "w+");
	FILE *npad = fopen(n_pad, "r");
	if(ifile != NULL && npad != NULL) {
		//Se lee cada caracter del archivo n_ifile
		while ((c_read = fgetc(ifile)) != EOF) {
			//Se lee el caracter de cifrado correspondiente
			c_random = fgetc(npad);
			//Se descifra el mensaje
			c_write = c_read ^ c_random;

			fputc(c_write, ofile);
		}
		//Cierre de sockets
		fclose(ifile);
		fclose(ofile);
		fclose(npad);
		return 0;
	}
	else {
		//Si no se pudo abrir n_ifile ni n_pad, es error.
		return -3;
	}
}

int main(int argc, char *argv[]){
	int errv;
	char n_ifile[20], n_ofile[20], n_pad[20];
	//Solo permite 3 o 4 argumentos al ejecutable
	if (argc <= 3 && argc >= 6) {
       printf("\nDa (especificamente) tres o cuatro argumentos al ejecutable\n");
        return -1;
    }

    strcpy(n_ifile, argv[1]);
    strcpy(n_ofile, argv[2]);
    strcpy(n_pad, argv[3]);

    //Si se dan 3 argumentos, se cifra el archivo
    //El orden de los argumentos es
    //./a.out archivo_a_cifrar archivo_desstino archivo_destino_de_cifrado
    if (argc == 4){
    	printf("Cifrando...\n");
    	errv = otpEncrypt(n_ifile, n_ofile, n_pad);
    	if (errv < 0) {
    		printf("Ocurrio un error al abrir el archivo $s\n", n_ifile);
    		return -2;
    	}
    }
    //Si se dan 4 argumentos, se descifra el archivo
    //El orden de los argumentos es
    // ./a.out archivo_a_descifrar archivo_destino archivo_de_cifrado bandera
    else {
    	printf("Descifrando...\n");
    	errv = otpDecrypt(n_ifile, n_ofile, n_pad);
    	if (errv < 0) {
    		printf("Ocurrio un error al abrir el (los) archivo(s):\n");
    		printf("%s o %s\n", n_ifile, n_pad);
    		return -3;
    	}
    }
    return 0;
}