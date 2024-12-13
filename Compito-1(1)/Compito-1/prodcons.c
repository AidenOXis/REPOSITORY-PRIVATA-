#include "prodcons.h"

#include <stdio.h>


void inizializza(MonitorStreaming * m) {

    /* TBD: Inizializzare il monitor */
    pthread_mutex_init(&m->mutex,NULL);
    pthread_cond_init(&m->cv_prod,NULL); 
    pthread_cond_init(&m->cv_cons,NULL);
    m->disponi=0; 
    
}

void produci(MonitorStreaming * m, Frame f) {

    /* TBD: Completare il metodo, con la sincronizzazione */
    pthread_mutex_lock(&m->mutex); 
    while(m->disponi == DIM){
        pthread_cond_wait(&m->cv_prod,&m->mutex); 
        
    }
    int pos = m->disponi; 
    printf("Produzione frame: { %d, %d, %d, %d }\n", f[0][0], f[0][1], f[1][0], f[1][1]);

    /* TBD */m->vettore[pos][0][0] = f[0][0];
    /* TBD */m->vettore[pos][0][1] =f[0][1];
    /* TBD */m->vettore[pos][1][0] = f[1][0];
    /* TBD */m->vettore[pos][1][1] = f[1][1];
    m->disponi++;
    pthread_cond_signal(&m->cv_cons); 
    pthread_mutex_unlock(&m->mutex); 

}

void consuma(MonitorStreaming * m, Frame f) {

    /* TBD: Completare il metodo, con la sincronizzazione */
    pthread_mutex_lock(&m->mutex);
    while(m->disponi == 0){
        pthread_cond_wait(&m->cv_prod,&m->mutex); 
        
    }
    int pos = m->disponi ; 
    f[0][0] = m->vettore[pos][0][0]; /* TBD */
    f[0][1] = m->vettore[pos][0][1];/* TBD */
    f[1][0] = m->vettore[pos][1][0];/* TBD */
    f[1][1] = m->vettore[pos][1][1];/* TBD */

    printf("Consumazione frame: { %d, %d, %d, %d }\n", f[0][0], f[0][1], f[1][0], f[1][1]);
    m->disponi--;
    pthread_cond_signal(&m->cv_prod); 
    pthread_mutex_unlock(&m->mutex); 
}

void bufferizza(MonitorStreaming * m, int n) {

    /* TBD: Completare il metodo */
    for(int i =0; i<n; i++){
        Frame f = {{i,i+1},{i+2,i+3}};
        produci(m,f); 
    }
   

}

void distruggi(MonitorStreaming * m) {

    /* TBD: Completare il metodo */
    pthread_mutex_destroy(&m->mutex); 
    pthread_cond_destroy(&m->cv_cons);
    pthread_cond_destroy(&m->cv_prod);

}