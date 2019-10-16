#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "impr.h"

void * imprimir_mult(void * ptr);

/* indique aca los campos que necesita en una orden de trabajo */
struct orden {
  Doc *doc;
  int listo;
};

/* defina aca las variables globales que necesite y programe las funciones
 * pedidas.
 */
ColaFifo *cola;
ColaFifo *threads;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t  condCola = PTHREAD_COND_INITIALIZER;
pthread_cond_t  condOrd = PTHREAD_COND_INITIALIZER;

int num_imprs;

void init_impr(Impr **imprs, int n_impr) {
  cola = nuevaColaFifo();
  threads = nuevaColaFifo();
  num_imprs = n_impr;

  /* use pthread_create para lanzar n_impr threads consumidores */
  for(int i=0; i<n_impr; i++){
    pthread_t t;
    pthread_create(&t,NULL,imprimir_mult, imprs[i]);
    agregar(threads,(void *)t);        //No debería haber datarace porque solamente es el hilo principal el que agrega
  }
}

void terminar_impr() {
  /* no olvide invocar pthread_join para enterrar los threads */
  for (int i=0;i<num_imprs;i++){
    pthread_mutex_lock(&m);
    agregar(cola,NULL);
    pthread_cond_broadcast(&condCola);
    pthread_mutex_unlock(&m);
  }

  while(!vacia(threads)){   //no debería haber datarace al sacar de threads, porque lo hace el principal solamente
    pthread_t t = (pthread_t)extraer(threads);
    pthread_join(t,NULL);
  }
  destruirColaFifo(cola);
  destruirColaFifo(threads);
}

Orden *imprimir_asinc(Doc *doc) {
  Orden *ord = malloc(sizeof(Orden));
  ord-> doc = doc;
  ord-> listo = 0;
  pthread_mutex_lock(&m);
  agregar(cola,ord);
  pthread_cond_broadcast(&condCola);
  pthread_mutex_unlock(&m);
  return ord;
}

void confirmar(Orden *ord) {
  pthread_mutex_lock(&m);
  while (!ord->listo)
  {
    pthread_cond_wait(&condOrd,&m);
  }
  free(ord);                  //Libera memoria
  pthread_mutex_unlock(&m);
}

/* no olvide programar ademas la funcion que ejecutara cada thread */

void * imprimir_mult(void * ptr){
  Impr * imprs = ptr;
  for(;;){
    pthread_mutex_lock(&m);
    while (vacia(cola))
     {
      pthread_cond_wait(&condCola,&m);
     }
    Orden * ord = extraer(cola);
    pthread_cond_broadcast(&condCola);
    pthread_mutex_unlock(&m);

    if(ord == NULL){
      break;
      return NULL;
    }

    imprimir(ord->doc,imprs);

    pthread_mutex_lock(&m);
    ord->listo = 1;
    pthread_cond_broadcast(&condOrd);
    pthread_mutex_unlock(&m);
  }
  return NULL;
}
