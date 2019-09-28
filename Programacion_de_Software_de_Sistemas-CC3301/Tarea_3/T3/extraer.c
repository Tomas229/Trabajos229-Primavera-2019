#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

void extraer(char* nombre_arch) {

	//Leer archivo
	FILE *arch = fopen(nombre_arch, "r+");

	//Ver número de elementos
	char buf[5];
	fread(buf, 1, 5, arch);
	int lineas = atoi(buf);

	//Preocuparse de underflow
	if (lineas < 1) {
		fprintf(stderr, "error %s está vacía \n", nombre_arch);
		exit(1);
	}

	//Buscar elemento a eliminar
	char buf2[17];
	int prioridad = INT_MAX;
	int linea_prioridad;

	for (int i = 0; i < lineas; i++) {
		fread(buf2, 1, 17, arch);
		char * marcador = buf2;
		marcador += 10;
		int prio = atoi(marcador);
		marcador -= 10;

		if (prio <= prioridad) {
			prioridad = prio;
			linea_prioridad = i;
		}
	}
	
	//imprimir eliminado
	char prioritario[17];
	fseek(arch, 4 + 1 + ((linea_prioridad)*(16 + 1)), SEEK_SET);
	fread(prioritario, 16, 1, arch);
	char * fin = prioritario;
	fin += 10;
	*fin = 0;
	printf("%s\n",prioritario);


	//Reemplaza el último en la cola por el eliminado
	fseek(arch, 4 + 1 + ((lineas - 1)*(16 + 1)),SEEK_SET);
	fread(buf2, 1, 17, arch);
	fseek(arch, 4 + 1 + ((linea_prioridad)*(16 + 1)), SEEK_SET);
	fwrite(buf2, 1, 17,arch);

	//Cambia el número de objetos en cola
	char s[6];
    int l = lineas-1;
	sprintf(s, "%-4d\n", l);
	fseek(arch,0, SEEK_SET);
	fwrite(s, 1, 5, arch);
}



int main(int argc, char **argv) {
	char *nom = argv[1];

	extraer(nom);
	return 0;
}