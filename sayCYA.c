#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>	//para listar los archivos en el directorio
#include <unistd.h>	//para listar los archivos del directorio
#include <sys/stat.h>	//para saber si un fichero es directorio
#include <sys/types.h>	//para saber si un fichero es directorio


#define length_path 100
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
long int getFileSize(FILE *stream){
	/*
		Funcion que obtiene el tamano de un archivo.
		Devuelve este valor.
	*/
	long int len;
	//Se posiciona al final el apuntador
	fseek(stream, 0, SEEK_END);
	//Se cuentan los bytes
	len = ftell(stream);
	//Se regresa el apuntador al inicio del archivo
	fseek(stream, 0, SEEK_SET);
	return len;
}

int encryptFile(char *file){
	FILE *natural_file;
	long int len;
	char *outFile;

	if((natural_file= fopen(file, "r")) != NULL){
		len = getFileSize(natural_file);
		//*outFile = malloc(len);
		printf("Tamano del archivo %s: %ld bytes\n", file, len);
	}
}

int isDir(char* target){
	/*
		Funcion que valida si un fichero es un directorio
		Retorna 1 si es directorio, 0 si es archivo comun
	*/
   struct stat statbuf;
   stat(target, &statbuf);
   return S_ISDIR(statbuf.st_mode);
}

char * getAbsPath(char *current_file, char *dir){
	/*
		Funcion que concatena el directorio de un fichero con el
		nombre del fichero para obtener la ruta absoluta de este.
		Retorna un apuntador a la cadena concatenada
	*/
	int len = strlen(current_file) + strlen(dir);
	//Se suman 2 espacios para el slash y el termino de cadena
	//calloc inicializa en cero el espacio reservado
	char *dest_file = calloc (len + 2,sizeof(char));
	strcat(dest_file, dir);
	strcat(dest_file, "/");
	strcat(dest_file, current_file);
	return dest_file;
}

int getFilesinDir(char *cwd){
	/*
		Funcion recursiva que lista todos los ficheros dentro del directorio
		dado como parametro. Cuando un fichero es un archivo comun, se imprime
		su tamano en bytes, cuando es un subdirectorio, se llama de manera
		recursiva a la funcion
	*/
	DIR *d;
    struct dirent *dir;
    char c, abs_path_file[length_path];
    d = opendir(cwd);
    //Si se tiene un apuntador diferente de NULL
    if (d){
    	//Se itera sobre todos los ficheros del directorio
        while ((dir = readdir(d)) != NULL)
        {
            printf("FILE: %s, %d\n", dir->d_name, isDir(dir->d_name));
            //Si es archivo comun, se llama a encryptFile para encriptarlo
            if (isDir(dir->d_name) == 0){
            	strcpy(abs_path_file, getAbsPath(dir->d_name, cwd));
            	encryptFile(abs_path_file);
            }
            //else if((strcmp(dir->d_name, ".") != 0)?(strcmp(dir->d_name, "..") != 0)?1:0:0){
            //Si es un subdirectorio, se llama de manera recursiva a la funcion con
            //la ruta absoluta del subdirectorio
            else if((strcmp(dir->d_name, ".") != 0)?(strcmp(dir->d_name, "..") != 0)?(strcmp(dir->d_name, ".git") != 0)?1:0:0:0){
            	//printf("\n\n%s\n", abs_path_file);
            	//memset(abs_path_file, 0, strlen(abs_path_file));
            	strcpy(abs_path_file, getAbsPath(dir->d_name, cwd));
            	printf("new p: %s\n", abs_path_file);
            	getFilesinDir(abs_path_file);
            }
        }
        closedir(d);
    }
    return 0;
}



int main(){
	char cwd[length_path];
	getcwd(cwd, (size_t) sizeof(cwd));
	getFilesinDir(cwd);
}