#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "prodcons.h"

void * produttore(void *);
void * consumatore(void *);

int main() {

    pthread_t thread_prod[5];
    pthread_t thread_cons;
    //pthread_attr_t attr;
    int rc;


    printf("[MAIN] inizializza_monitor\n");

    /* TBD: Creare e inizializzare il monitor */
     MonitorProdCons *monitor = malloc(sizeof(MonitorProdCons));
    inizializza_monitor(monitor);


    printf("[MAIN] pthread_create\n");

    /* TBD: Avviare i thread */

   // pthread_attr_init(&attr);
    //pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

    for(int i = 0;i<5;i++){
        rc = pthread_create(&thread_prod[i],NULL,produttore,(void *)monitor);
    }
    rc= pthread_create(&thread_cons, NULL, consumatore, (void *)monitor);



    printf("[MAIN] pthread_join\n");

    /* TBD: Attendere la terminazione dei thread */
   for(int i = 0 ; i<5;i++){
    pthread_join(thread_prod[i],NULL);
   }
   pthread_join(thread_cons,NULL);
    


    printf("[MAIN] distruzione_monitor\n");

    /* TBD: Disattivazione e deallocazione del monitor */
    distruggi_monitor(monitor);
    free(monitor);

}


void * produttore(void * p) {
    MonitorProdCons *monitor = (MonitorProdCons *)p;

    printf("[MAIN] Avvio thread produttore\n");


    for(int i=0; i<4; i++) {

        int val = rand() % 10;

        printf("[MAIN] Produttore ha generato %d\n", val);

        int ret = produci(monitor,val);/* TBD: Chiamare il metodo di produzione */

        while(ret < 0) {

            sleep(1);

            printf("[MAIN] Produttore ritenta inserimento di %d\n", val);

            ret =produci(monitor,val); /* TBD: Chiamare il metodo di produzione */
        }

    }


    pthread_exit(NULL);
}

void * consumatore(void * p) {
     MonitorProdCons *monitor = (MonitorProdCons *)p;

    printf("[MAIN] Avvio thread consumatore\n");


    for(int i=0; i<20; i++) {

        int val;

        int ret = consuma(monitor,&val);/* TBD: Chiamare il metodo di consumazione */

        printf("[MAIN] Consumatore ha prelevato %d\n", val);

        sleep(2);
    }


    pthread_exit(NULL);
}