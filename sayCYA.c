#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>	//para listar los archivos en el directorio
#include <unistd.h>	//para listar los archivos del directorio
#include <sys/stat.h>	//para saber si un fichero es directorio
#include <sys/types.h>	//para saber si un fichero es directorio
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 6789  
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
#if 0
int isDir(char* target){
	/*
		Funcion que valida si un fichero es un directorio
		Retorna 1 si es directorio, 0 si es archivo comun
	*/
   struct stat statbuf;
   stat(target, &statbuf);
   return S_ISDIR(statbuf.st_mode);
}
#endif
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
						printf("FILE: %s, %d\n", dir->d_name, dir->d_type);
						//printf("FILE: %s, %d\n", dir->d_name, isDir(dir->d_name));
            //Si es archivo comun, se llama a encryptFile para encriptarlo
            //if (isDir(dir->d_name) == 0){
						// Si el tipo de archivo es igual a 8 se trata de un archivo comun
					 	if (dir->d_type == 8){ // Es un archivo
							strcpy(abs_path_file, getAbsPath(dir->d_name, cwd));
            	encryptFile(abs_path_file);
            }
            //else if((strcmp(dir->d_name, ".") != 0)?(strcmp(dir->d_name, "..") != 0)?1:0:0){
            //Si es un subdirectorio, se llama de manera recursiva a la funcion con
            //la ruta absoluta del subdirectorio
						// Si el tipo de archivo es igual a 4 se trata de un directorio
						else if (dir->d_type == 4){ // Es un directorio
								if((strcmp(dir->d_name, ".") != 0)?(strcmp(dir->d_name, "..") != 0)?(strcmp(dir->d_name, ".git") != 0)?1:0:0:0){
	            	//printf("\n\n%s\n", abs_path_file);
	            	//memset(abs_path_file, 0, strlen(abs_path_file));
		            	strcpy(abs_path_file, getAbsPath(dir->d_name, cwd));
		            	printf("new p: %s\n", abs_path_file);
		            	getFilesinDir(abs_path_file);
	            	}
						}
        }
        closedir(d);
    }
    return 0;
}

int conecta(){
	int sockfd, new_sockfd;  // descriptores de archivo
	//Estructura para guardar los datos necesarios para el socket
	struct sockaddr_in host_addr, client_addr;  // Informacion de las direcciones IP
	socklen_t sin_size;
	int recv_length=1, yes=1;

	char buffer[6];
	char instr[8];
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		perror("Error al crear el socket");

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		perror("Error al agregar la opcion SO_REUSEADDR en setsockopt");//----Mensaje que se quita en cuanto termine la creacion-----

	//Establece la familia correspondiente al protocolo
	host_addr.sin_family = AF_INET;     // 
	//big endian 
	host_addr.sin_port = htons(PORT);   //
	host_addr.sin_addr.s_addr = INADDR_ANY; // Asigno mi IPP
	memset(&(host_addr.sin_zero), '\0', 8); // El resto de la estructura en 0s
	//escucha las peticiones.
	if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1)
		perror("Error haciendo el bind");//----Mensaje que se quita en cuanto termine la creacion------

	if (listen(sockfd, 5) == -1)
		perror("Error al escuchar en el socket");//----Mensaje que se quita en cuanto termine la creacion-----

	while(1) { 
		sin_size = sizeof(struct sockaddr_in);
		new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
		if(new_sockfd == -1)
			perror("Error al aceptar la conexion");//----Mensaje que se quita en cuanto termine la creacion-----
		send(new_sockfd, "Conexion aceptada. Comienza a teclear\n", 38, 0);
		recv_length = recv(new_sockfd, &buffer, 6, 0);
		while(recv_length > 0) {
			printf("RECV: %d bytes\nENTRADA: %s...", recv_length, buffer);//----Mensaje que se quita en cuanto termine la creacion-----
			if(strcmp(buffer, "encrypt\n") == 0)//  command: encr
				/*Aqui iria la funcion de cifrado*/
				send(new_sockfd, "Escogio encriptar\n", 38, 0);
			else if(strcmp(buffer, "decrypt\n") == 0)//  command: decr
				/*Aqui iria la funcion de descifrado*/
				send(new_sockfd, "Escogio desencriptar\n", 38, 0);
			else if(strcmp(buffer,"list\n") == 0)//  command: list
				/*Aqui iria la funcion de listado de archivos para obtener info*/
				send(new_sockfd, "Escogio listar los archivos\n", 38, 0);
			else if(strcmp(buffer,"stop\n") == 0){//  command: stop
				/*Aqui iria la funcion de  desconectar el socket*/
				send(new_sockfd, "detener todo\n", 38, 0);
				return 1;
			}
			else
				send(new_sockfd, "No se proceso correctamente\n", 38, 0);
			
			memset(buffer, 0, recv_length);
			recv_length = recv(new_sockfd, &buffer, 6, 0);
		}
	close(new_sockfd);
	}
  return 0;
}


int main(){
	int a= conecta();
//	char cwd[length_path];
	//getcwd(cwd, (size_t) sizeof(cwd));
//	getFilesinDir(cwd);
}
