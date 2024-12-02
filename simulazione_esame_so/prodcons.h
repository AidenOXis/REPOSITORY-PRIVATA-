#ifndef _PRODCONS_H_
#define _PRODCONS_H_

#include <pthread.h>

#define DIM 4

typedef struct {

    int buffer[DIM];

    /* TBD: Definire variabili per la sincronizzazione,
     *      e una variabile "uscita_timer"
     */
    pthread_mutex_t mutex; 
    pthread_cond_t cv_prod;
    pthread_cond_t cv_cons;

    int uscita_timer;

    int testa;
    int coda;
    int count;

    int thread_prod;
    int thread_cons;

} MonitorProdCons;

void inizializza_monitor(MonitorProdCons * m);
void distruggi_monitor(MonitorProdCons * m);
int produci(MonitorProdCons * m, int val);
int consuma(MonitorProdCons * m, int * val);

#endif