#include "header_monitor.h"

void init_monitorpc(MonitorPC *p) {

    /* TBD: Inizializzare le variabili testa/coda per il vettore circolare */
    p->testa = 0 ; 
    p->coda=0; 
    p->count=0; 
    p->thread_produttore = 0; 
    p->thread_consumatore = 0 ; 



    /* TBD: Inizializzare le variabili per la sincronizzazione */
    pthread_mutex_init(&p->mutex,NULL); 
    pthread_cond_init(&p->cv_produttore,NULL); 
    pthread_cond_init(&p->cv_consumatore,NULL);
}

void remove_monitorpc(MonitorPC *p) {

    /* TBD: De-inizializzare le variabili per la sincronizzazione */
    pthread_mutex_destroy(&p->mutex); 
    pthread_cond_destroy(&p->cv_consumatore); 
    pthread_cond_destroy(&p->cv_produttore); 

}

void produci(MonitorPC *p, int val) {

    /* TBD: Completare il metodo con la sincronizzazione
            e con la gestione della coda circolare
     */
    pthread_mutex_lock(&p->mutex); 
    while (p->count==DIM)
    {
        p->thread_produttore++; 
        pthread_cond_wait(&p->cv_produttore,&p->mutex); 
        p->thread_produttore--; 
    }
  
    p->buffer[p->testa/* TBD */] = val;
    p->testa = (p->testa + 1 ) % DIM; 
    p->count++; 


    printf("Produttore ha prodotto %d\n",val);
    pthread_cond_signal(&p->cv_consumatore); 
    pthread_mutex_unlock(&p->mutex);  

}

int consuma(MonitorPC *p) {

    int val;

    /* TBD: Completare il metodo con la sincronizzazione
            e con la gestione della coda circolare
     */
    pthread_mutex_unlock(&p->mutex); 
    while(p->count == 0 ){
        p->thread_consumatore++; 
        pthread_cond_wait(&p->cv_consumatore,&p->mutex); 
        p->thread_consumatore--; 
    }
     


   



    val = p->buffer[p->coda/* TBD */];
    p->coda = (p->coda + 1 ) % DIM; 
    p->count++; 
    printf("Consumatore ha consumato %d\n",val);
    pthread_cond_signal(&p->cv_produttore); 
    pthread_mutex_unlock(&p->mutex);  



    return val;
}
