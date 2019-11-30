#include <stdio.h>
#include <pthread.h>

#include "integral.h"

typedef struct {
    Funcion f;
    void *ptr;
    double xi, xf;
    int n;
    double resultado;
}Args;


void * integral_thread(void * ptr){
    Args * arg = ptr;
    double num = integral(arg->f, arg->ptr, arg->xi, arg->xf, arg->n);
    printf("%lf\n", num);
    arg->resultado = num;
    return NULL;
}

double integral_par(Funcion f, void *ptr, double xi, double xf, int n, int p) {
    pthread_t t[p];
    Args array[p];

    double dx = (xf-xi)/p;

    int k = n%p;

    double resultadoFinal = 0;

    for(int r=0; r<p; r++){
        Args *arg = & array[r];
        arg->f = f; arg->ptr = ptr; 
        
        int i = n/p;
        if(r<k){
            i++;
        }
        arg->n = i;

        arg->xi = xi + r*dx;
    
        arg->xf = xi + (r+1)*dx;

        pthread_create(&t[r], NULL, integral_thread, arg);
    }

    for(int r=0; r<p; r++){
        pthread_join(t[r], NULL);
        Args *arg = & array[r];
        resultadoFinal += arg->resultado;
    }
    
    return resultadoFinal;
}