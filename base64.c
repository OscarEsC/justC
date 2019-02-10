/*
	Practica 13: Codificacion base64
	Autor: Espinosa Curiel Oscar
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Encodes Base64
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <assert.h>

//Fork de https://gist.github.com/barrysteyn/7308212

int base64Encode(const unsigned char* buffer, char** b64text) {
	/*
		Funcion que codifica el texto del primer argumento a base64
		copiando el resultado en el apuntador del segundo argumento
		Retorna 0 si se ejecuto correctamente.
	*/
	BIO *bio, *b64;
	BUF_MEM *bufferPtr;
	size_t length = strlen(buffer);

	b64 = BIO_new(BIO_f_base64());
	bio = BIO_new(BIO_s_mem());
	bio = BIO_push(b64, bio);
	//No escribe saltos de linea en el texto codificado
	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
	BIO_write(bio, buffer, length);
	BIO_flush(bio);
	BIO_get_mem_ptr(bio, &bufferPtr);
	BIO_set_close(bio, BIO_NOCLOSE);
	BIO_free_all(bio);

	*b64text=(*bufferPtr).data;
	return (0);
}

size_t calcDecodeLength(const char* b64input) {
	/*
		Funcion que calcula la longitud de la cadena decodificada
		Retorna el tamano calculado
		*/
	size_t len = strlen(b64input),
		padding = 0;

	if (b64input[len-1] == '=' && b64input[len-2] == '=')
		padding = 2;
	else if (b64input[len-1] == '=')
		padding = 1;

	return (len*3)/4 - padding;
}

int base64Decode(char* b64message, char** buffer, size_t* length) {
	/*
		Funcion que decodifica una cadena (dada como primer argumento).
		El resultado lo copia en el apuntador dado como segundo argumento.
		En el tercer argumento escribe la longitud del texto decodificado.
		Retorna 0 si se ejecuto correctamente.
		*/
	BIO *bio, *b64;
	int decodeLen = calcDecodeLength(b64message);
	*buffer = (unsigned char*)malloc(decodeLen + 1);
	(*buffer)[decodeLen] = '\0';

	bio = BIO_new_mem_buf(b64message, -1);
	b64 = BIO_new(BIO_f_base64());
	bio = BIO_push(b64, bio);
	//Para no escribir saltos de linea en el texto decodificado
	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
	*length = BIO_read(bio, *buffer, strlen(b64message));
	//Si las longitudes no son iguales, lanza un error.
	assert(*length == decodeLen);
	BIO_free_all(bio);

	return (0);
}

int main(int argc, char **argv){
	char *digestText;
	size_t lengthDecode;
	//Si solo se da un argumento al ejecutable, se codifica el argumento
	if (argc ==2){
		base64Encode(*(argv + 1), &digestText);
		printf("Encode Text: \n");
	}
	//Si se dan dos argumentos, se decodifica el primero de ellos.
	//El segundo de ellos es indiferente en la ejecucion
	else if (argc == 3){
		base64Decode(*(argv + 1), &digestText, &lengthDecode);
		printf("Decode Text:\n");
	}

	printf("%s\n", digestText);

	return 0;
}

