// Coloque aca los #include que necesite
// Si usa por ejemplo strcpy, ingrese en el shell de comandos:
// $ man 3 strcpy
// En SYNOPSIS le indicara que debe incluir el archivo de encabezados string.h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "t1.h"

// Programe aca las 3 funciones pedidas

uint comprimir(uint *a, int nbits){

	int lenUint=sizeof(uint)<<3;
	
	uint final=0;
	uint mask = ~(-1<<nbits);

	for (int i = nbits; i <= lenUint; i += nbits) {
		final = final << nbits;
		uint m = *a & mask;
		final = final|m;
		a++;
	}
	return final;
}

char * insercion(char *d, char *s){

	int len1 = strlen(d);			
	int len2 = strlen(s);
	int len3 = len1 + len2 + 1;
	
	char * a = malloc(sizeof(char)*len3);
	
	strcpy(a,s);
	strcat(a,d);
	

	return a;
}

void insertar(char *d, char *s) {
	int len1 = strlen(d);
	int len2 = strlen(s);

	char * a = d;

	a = a + len1 + len2;
	d = d + len1 ;

	for (int i = 0; i < len1+1; i++) {
		*a = *d;
		a = a - 1;
		d = d - 1;
	}
	d++;
	a++;
	for (int i = 0; i < len2; i++) {
		*d = *s;
		d++;
		s++;
	}
}

