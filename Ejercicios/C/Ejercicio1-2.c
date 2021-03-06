#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

void
mayusculas (char* buffer){
	
	int i;
	for (i = 0; i < strlen(buffer); i++){
		buffer[i] = toupper(buffer[i]);
	}
	
}


int
main (void){
	
	// Variables
	
	pid_t PID;
	int p[2][2];
	
	char buffer[1024];
	
	FILE* fr;
	FILE* fw;
	
	// Fin Variables
	
	pipe(p[0]);
	pipe(p[1]);
	
	PID = fork();
	if (PID < 0){
		
		fprintf(stderr, "Error al crear el proceso hijo");
		exit(1);
	
	} else if (PID == 0){ // Proceso Hijo
		
		close(p[0][1]);
		close(p[1][0]);
		
		fr = fdopen(p[0][0], "r");
		fw = fdopen(p[1][1], "w");
		
		while (fgets(buffer, 1024, fr) != NULL){
			
			mayusculas(buffer);
			fputs(buffer, fw);
			fflush(fw);
			
		}
		
		fclose(fw);
		fclose(fr);
		exit(0);
		
	} else { // Proceso Padre
		
		close(p[0][0]);
		close(p[1][1]);
		
		fw = fdopen(p[0][1], "w");
		fr = fdopen(p[1][0], "r");
		
		while(fgets(buffer, 1024, stdin) != NULL){
			
			fputs(buffer, fw);
			fflush(fw);
			fgets(buffer, 1024, fr);
			fputs(buffer, stdout);
		}
		
		fclose(fw);
		fclose(fr);
		wait(NULL);
		
	}
	
	exit(0);
}
