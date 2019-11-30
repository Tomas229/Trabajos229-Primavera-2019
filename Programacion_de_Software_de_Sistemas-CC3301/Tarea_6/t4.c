#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "integral.h"

int leer(int fd, void *buf, int n) {
  if (n==0)
    return 0;
  do {
    int rc= read(fd, buf, n);
    if (rc<=0)
      return 1; /* fracaso: error o fin del archivo/pipe/socket */
    n-= rc; /* descontamos los bytes leídos */
    buf= (char*)buf + rc; /* avanzamos el buffer */
  } while (n>0); /* mientras no leamos todo lo que esperamos */
  return 0; /* exito */
}

double integral_par(Funcion f, void *ptr, double xi, double xf, int n, int p) {
    int pids[p];
    int infds[p];

    double dx = (xf-xi)/p;
    int k = n%p;

    for (int r= 0; r<p; r++) {
        int fds[2];
        pipe(fds);            

        int i = n/p;         
        if(r<k){
            i++;
        }


        if ((pids[r]= fork())==0) { 
            close(fds[0]);
            double *res; 
            double num = integral(f, ptr, xi + r*dx, xi + (r+1)*dx, i);
            res = & num; 
            write(fds[1], res , sizeof(double));        //arreglar
            exit(0);   
        }
        else {
            close(fds[1]);
            infds[k] = fds[0];
        }
    }
    double resultadoFinal = 0;
    for (int k= 0; k<p; k++) {
        double buf[1];              
        leer(infds[k], buf, sizeof(double));
        waitpid(pids[k], NULL, 0);  
        close(infds[k]); 
        printf("%lf\n", buf[0]);
        resultadoFinal += buf[0];
        }

    return resultadoFinal;
}

