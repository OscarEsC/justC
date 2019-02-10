/*
	Pratica 11: Sockets en C. Protocolo INSTRUCT
	Autor: Espinosa Curiel Oscar
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Estructura sockaddr
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<unistd.h>

#define PORT 6789  // puerto de conexion

char *instruct(char *received, char *cp){
	/*
		Funcion que mapea frases tipicas con sus autores
		compara el primer argumento con la frase tipica y
		copia el autor en el segundo argumento. Devuelve
		"nb" si no existe autor de esa frase.
	*/
	if(strcmp(received, "ninios\n") == 0)
		strcpy(cp,"Quintero");
	else if(strcmp(received, "no se\n") == 0)
		strcpy(cp,"Angie");
	else if(strcmp(received,"...\n") == 0)
		strcpy(cp,"Veronica");
	else
		strcpy(cp,"nb");
}

int main(void) {
  int sockfd, new_sockfd;  // descriptores de archivo
  //Estructura para guardar los datos necesarios para el socket
  struct sockaddr_in host_addr, client_addr;  // Informacion de las direcciones IP
  //tamano para conexiones de red
  socklen_t sin_size;
  int recv_length=1, yes=1;

  char buffer[1024];
  char instr[8];
  //PF_INET es una familia de protocolos (macros) ipv4, x25, etc
  //SOCK_STREAM define que vamos a trabajar con un socket de flujo
  if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    perror("Error al crear el socket");

  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    perror("Error al agregar la opcion SO_REUSEADDR en setsockopt");
  
  //Establece la familia correspondiente al protocolo
  host_addr.sin_family = AF_INET;     // 
  //big endian 
  host_addr.sin_port = htons(PORT);   //
  //Macro o constante que define que es nuestra ip
  host_addr.sin_addr.s_addr = INADDR_ANY; // Asigno mi IPP
  memset(&(host_addr.sin_zero), '\0', 8); // El resto de la estructura en 0s
  //escucha las peticiones.
  if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1)
    perror("Error haciendo el bind");

	//
  if (listen(sockfd, 5) == -1)
    perror("Error al escuchar en el socket");

  while(1) {    // Accept loop
    sin_size = sizeof(struct sockaddr_in);
    new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
    if(new_sockfd == -1)
      perror("Error al aceptar la conexion");
    printf("server: Conexion aceptada desde %s desde  %d\n",inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    send(new_sockfd, "Conexion aceptada. Comienza a teclear\n", 38, 0);
    recv_length = recv(new_sockfd, &buffer, 1024, 0);
    while(recv_length > 0) {
      printf("RECV: %d bytes\n", recv_length);
      instruct(buffer, instr);
      if (strcmp(instr, "nb") != 0)
      	printf("Frase de %s\n",instr);
      else
      	printf("Recibiendo: %s\n",buffer);
      //Borramos el buffer para una lectura limpia
      memset(buffer, 0, recv_length);
      //nueva lectura del socket
      recv_length = recv(new_sockfd, &buffer, 1024, 0);
    }
    close(new_sockfd);
  }
  return 0;
}