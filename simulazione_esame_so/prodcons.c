#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

#include "prodcons.h"

/* Dichiarazione di metodo privato, che sarà
 * eseguito da un thread "timer" */
static void * thread_timer(void *);


void inizializza_monitor(MonitorProdCons * m) {

    printf("[MONITOR] Inizializzazione monitor\n");

    /* TBD: Inizializzazione il monitor, e avviare
     *      il thread timer
     */
    pthread_mutex_init(&m->mutex,NULL);
    pthread_cond_init(&m->cv_cons,NULL);
    pthread_cond_init(&m->cv_prod,NULL);
    
    m->count = 0 ; 
    m->testa = 0; 
    m-> coda =  0 ;
    m->thread_prod=0; 
    m->thread_cons = 0; 
    m->uscita_timer = 0; 

    pthread_t timer;
    pthread_create(&timer,NULL,thread_timer,(void *)m);
    //pthread_attr_t attr;
    //pthread_attr_init(&attr);
    //pthread_attr_getdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
    

}

void distruggi_monitor(MonitorProdCons * m) {

    printf("[MONITOR] Distruzione monitor\n");

    /* TBD: Disattivare il monitor, e far
     *      terminare il thread timer tramite
     *      la variabile "uscita_timer"
     */
    //pthread_mutex_lock(&m->mutex);
    m->uscita_timer = 1; 
    //pthread_mutex_unlock(&m->mutex);
    pthread_mutex_destroy(&m->mutex);
    pthread_cond_destroy(&m->cv_cons);
    pthread_cond_destroy(&m->cv_prod);

}

int produci(MonitorProdCons * m, int val) {

    /* TBD: Introdurre la sincronizzazione */
    pthread_mutex_lock(&m->mutex);
    while(m->count == DIM)
    {
        m->thread_prod++;
        pthread_cond_wait(&m->cv_cons,&m->mutex);
        m->thread_prod--;      
    

    

    // PRIMA DELL'ATTESA, SALVA L'ORARIO
    struct timeval t1;
	gettimeofday(&t1,NULL);
    long start_time = t1.tv_sec*1000000 + t1.tv_usec;
	}

    // DOPO L'ATTESA, SALVA DI NUOVO L'ORARIO, E CONTROLLA SE SONO PASSATI 3 SECONDI
    struct timeval t2;
    gettimeofday(&t2,NULL);
    long current_time = t2.tv_sec*1000000 + t2.tv_usec;

    if(current_time - start_time >= 3000000) {

        printf("[MONITOR] Produttore ha atteso oltre 3 secondi, esce\n");

        /* TBD: Il metodo esce dal monitor senza porsi nuovamente 
                in attesa sulla condition variable, e ritorna in uscita
                un valore negativo
            */
           pthread_mutex_unlock(&m->mutex);

            return -1; 

    }
    



    m->buffer[m->testa/*TBD*/] = val;
    m->testa = (m->testa + 1) %DIM;
    m->count ++; 

    printf("[MONITOR] Produzione %d\n", val);
    pthread_cond_signal(&m->cv_prod);
    pthread_mutex_unlock(&m->mutex);

    
    return 0;
}

int consuma(MonitorProdCons * m, int * val) {

    /* TBD: Introdurre la sincronizzazione */
    pthread_mutex_lock(&m->mutex);
    while(m->count == 0)
    {
        m->thread_cons++;
        pthread_cond_wait(&m->cv_prod,&m->mutex);
        m->thread_cons--;      
    }


    struct timeval t1;
	gettimeofday(&t1,NULL);
    long start_time = t1.tv_sec*1000000 + t1.tv_usec;



        struct timeval t2;
	    gettimeofday(&t2,NULL);
        long current_time = t2.tv_sec*1000000 + t2.tv_usec;

        if(current_time - start_time >= 3000000) {

            printf("[MONITOR] Consumatore ha atteso oltre 3 secondi, esce\n");

            /* TBD: Il metodo esce dal monitor senza porsi nuovamente 
                    in attesa sulla condition variable, e ritorna in uscita
                    un valore negativo
             */
            pthread_mutex_unlock(&m->mutex);
            return -1; 
        }



    *val = m->buffer[m->coda/*TBD*/];
    m->coda = (m->coda +1 ) %DIM;
    m->count--;

    printf("[MONITOR] Consumazione %d\n", *val);

    pthread_cond_signal(&m->cv_cons);
    pthread_mutex_unlock(&m->mutex);
    return 0;
}

void * thread_timer(void * p) {

    printf("[MONITOR] Avvio thread timer\n");
    MonitorProdCons * m = (MonitorProdCons *) p;


    while(1) {

        int uscita = 0;

        sleep(1);

        pthread_mutex_lock(&m->mutex);
        /* TBD: Il thread timer periodicamente attiva tutti i thread sospesi */
        pthread_cond_broadcast(&m->cv_prod);
        pthread_cond_broadcast(&m->cv_cons);

        pthread_mutex_unlock(&m->mutex);


        if(uscita != 0) {
            break;
        }
    }

    printf("[MONITOR] Uscita thread timer\n");

    pthread_exit(NULL);
}
