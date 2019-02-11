/*
	Autor: Espinosa Curiel Oscar
	Socket cliente para la conexion con socket.c
*/

#include <stdio.h>
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>// inet_pton
#include<unistd.h>//close
#define PORT 6789
#define server_address "127.0.0.1"

int main(int argc, char const *argv[]) 
{ 
	struct sockaddr_in address; 
	int sock = 0, valread, recv_length=1;
	struct sockaddr_in serv_addr;
	char msg[1024];
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Error en la creacion del socket \n"); 
		return -1; 
	} 

	memset(&serv_addr, '0', sizeof(serv_addr)); 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, server_address, &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nDireccion invalida o no soportada \n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConeccion Fallida \n"); 
		return -1; 
	}

	//Reepcion del mensaje de conexion enviado por el servidor
	recv_length = recv(sock, &msg, 1024, 0);
	printf("%s\n", msg);
    while(recv_length > 0) {
      //Lectura de stdin
      fgets(msg, 1024, stdin);
      //Envio del mensaje al servido
      send(sock, msg, strlen(msg), 0);
      //Se limpia el buffer
      memset(msg, 0, strlen(msg));
    }
    close(sock);

	return 0; 
} 