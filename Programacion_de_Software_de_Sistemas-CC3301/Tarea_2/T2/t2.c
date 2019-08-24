#include "t2.h"
#include <stddef.h>



int calzar(Nodo *a, Nodo **ppat) {
	Nodo *b = *ppat;
	if (b == NULL) {    //Ejemplo i)
		*ppat = a;
		return 1;
	}

	if (a == NULL && b != NULL) { //Ejemplo ii)
		return 0;
	}

	if (a->x != b->x) {			//Ejemplo iii)
		return 0;
	}

	return calzar(a->izq, &b->izq) & calzar(a->der, &b->der);
}