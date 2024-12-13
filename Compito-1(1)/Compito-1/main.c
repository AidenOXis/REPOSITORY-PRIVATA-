#include "prodcons.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void * Produttore(void *);
void * Consumatore(void *);

int main() {

    /* TBD: Avviare i thread produttore e consumatore */
    MonitorStreaming monitor; 
    
    pthread_t prod_thread;
    pthread_t cons_thread; 
   
    pthread_create(&prod_thread,NULL,Produttore,&monitor);
    pthread_create(&cons_thread,NULL,Consumatore,&monitor);
    
    pthread_join(prod_thread,NULL); 
    pthread_join(cons_thread,NULL); 

    distruggi(&monitor);
    return 0; 

}

void * Produttore(void * p) {

    MonitorStreaming * monitor = (MonitorStreaming *)p; 
    for(int i=0; i<10; i++) {

        Frame f;
        f[0][0] = rand() % 11;
        f[0][1] = rand() % 11;
        f[1][0] = rand() % 11;
        f[1][1] = rand() % 11;

        /* TBD: Chiamare il metodo "produci()" del monitor */
        produci(monitor,f); 
        sleep(1);
    }

    pthread_exit(NULL);
}

void * Consumatore(void * p) {

    MonitorStreaming * monitor = (MonitorStreaming *)p;

    /* TBD: Chiamare il metodo "bufferizza()" del monitor */
    bufferizza(monitor,10); 
    for(int i=0; i<10; i++) {

        Frame f;

        /* TBD: Chiamare il metodo "consuma()" del monitor */
        consuma(monitor,f);
        sleep(1);
    }

    pthread_exit(NULL);
}