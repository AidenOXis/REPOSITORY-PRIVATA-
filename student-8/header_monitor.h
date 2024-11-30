#ifndef _HEADER_MONITOR_H_
#define _HEADER_MONITOR_H_

#include <pthread.h>

#define DIM 5

typedef struct {

    int buffer[DIM];

    /* TBD: Aggiungere le variabili testa/coda per il vettore circolare */
    int testa; 
    int coda; 
    int count; 
    int thread_produttore; 
    int thread_consumatore; 

    /* TBD: Aggiungere le variabili per la sincronizzazione */
    pthread_mutex_t mutex; 
    pthread_cond_t cv_produttore; 
    pthread_cond_t cv_consumatore; 

} MonitorPC;

void init_monitorpc(MonitorPC *p);
void remove_monitorpc(MonitorPC *p);
void produci(MonitorPC *p, int val);
int consuma(MonitorPC *p);

#endif