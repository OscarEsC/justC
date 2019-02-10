/*
	Practica 9: Numero pseudo-aleatorio
	Autor: Espinosa Curiel Oscar
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int fd;
	unsigned int *c = malloc(sizeof(unsigned int));
	char *archivo = "/dev/random";
	fd = open(archivo, O_RDONLY,S_IRUSR|S_IWUSR);
	read(fd, c, sizeof(unsigned int));
	close(fd);
	printf("%lu\n", *c%10);
	return 0;
}