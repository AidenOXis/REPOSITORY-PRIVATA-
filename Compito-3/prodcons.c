#include "prodcons.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>


void inizializza(MonitorStreaming * p) {

    /* TBD: Inizializzare il monitor */
    init_monitor(&p->monitor, 2); 

    for(int i = 0; i < DIM; i++) {
        p->vettore[i].stato = LIBERO;
    }
    p->testa = 0 ;
    p->coda = 0 ; 
    p->count = 0 ; 

}


void produci(MonitorStreaming * p, char * stringa, size_t lunghezza, key_t chiave) {

    /* TBD: Completare il metodo, con la sincronizzazione */
    enter_monitor(&p->monitor); 



    printf("Avvio produzione...\n");
    if(p->count == DIM){
        wait_condition(&p->monitor, CV_PRODUTTORE);
    }
    int i = 0 ;
    while (i<DIM && p->vettore[p->testa].stato != LIBERO){
        p->testa=(p->testa+1)%DIM;
    }

    sleep(1);
  
   
    p->vettore[p->testa].stato=IN_USO;
    
    

    int shmid = shmget(chiave,lunghezza + 1, IPC_CREAT | 0664);
    char * stringa_shm = shmat(shmid,NULL,0); /* TBD: Creare una nuova shared memory, su cui copiare la stringa */

    /* TBD */ p->vettore[p->testa].dimensione= lunghezza;
    /* TBD */ p->vettore[p->testa].chiave= chiave;
    /* TBD */ p->vettore[p->testa].produttore= getpid();
    p->vettore[p->testa].stato =OCCUPATO;

    strcpy(stringa_shm, stringa);   // Copia della stringa
    shmdt(stringa_shm);
   
    p->testa=(p->testa+1)%DIM;
    p->count ++; 
    

    printf("Produzione completata: %s (%zu char, key=%x)\n", stringa_shm, lunghezza, chiave);
    signal_condition(&p->monitor, CV_CONSUMATORE);
    leave_monitor(&p->monitor);
}

void consuma(MonitorStreaming * p, char * stringa, size_t * lunghezza) {

    /* TBD: Completare il metodo, con la sincronizzazione */
    enter_monitor(&p->monitor);

    printf("Avvio consumazione...\n");
    if(p->count == 0){
        wait_condition(&p->monitor,CV_CONSUMATORE);
    }
    int i = 0 ; 
    while(p->vettore[i].stato != OCCUPATO){
        p->coda=(p->coda+1)%DIM;
    }

    sleep(1);
    
    p->vettore[p->coda].stato=IN_USO;
    


    key_t chiave = p->vettore[p->coda].chiave;/* TBD */
    pid_t produttore = p->vettore[p->coda].produttore/* TBD */;
    *lunghezza = p->vettore[p->coda].dimensione/* TBD */;

    int shmid = shmget(chiave, *lunghezza +1 , IPC_CREAT | 0664);
    char * stringa_shm = shmat(shmid,NULL,0); /* TBD: Accedere alla stringa su shared memory */

    strcpy(stringa, stringa_shm);   // Copia della stringa
    shmdt(stringa_shm);

    p->vettore[p->coda].stato = LIBERO;
    p->coda=(p->coda+1)%DIM;
    p->count --; 

    printf("Consumazione completata: %s (%zu char, key=%x)\n", stringa, *lunghezza, chiave);
    signal_condition(&p->monitor,CV_PRODUTTORE);
    leave_monitor(&p->monitor);
}

void distruggi(MonitorStreaming * p) {

    /* TBD: Completare il metodo */
    #include "prodcons.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>


void inizializza(MonitorStreaming * p) {

    /* TBD: Inizializzare il monitor */
    init_monitor(&p->monitor, 2); 

    for(int i = 0; i < DIM; i++) {
        p->vettore[i].stato = LIBERO;
    }
    p->testa = 0 ;
    p->coda = 0 ; 
    p->count = 0 ; 

}


void produci(MonitorStreaming * p, char * stringa, size_t lunghezza, key_t chiave) {

    /* TBD: Completare il metodo, con la sincronizzazione */
    enter_monitor(&p->monitor); 



    printf("Avvio produzione...\n");
    if(p->count == DIM){
        wait_condition(&p->monitor, CV_PRODUTTORE);
    }
    int i = 0 ;
    while (i<DIM && p->vettore[p->testa].stato != LIBERO){
        p->testa=(p->testa+1)%DIM;;
    }

    sleep(1);
  
   
    p->vettore[p->testa].stato=IN_USO;
    
    

    int shmid = shmget(chiave,lunghezza + 1, IPC_CREAT | 0664);
    char * stringa_shm = shmat(shmid,NULL,0); /* TBD: Creare una nuova shared memory, su cui copiare la stringa */

    /* TBD */ p->vettore[p->testa].dimensione= lunghezza;
    /* TBD */ p->vettore[p->testa].chiave= chiave;
    /* TBD */ p->vettore[p->testa].produttore= getpid();
    p->vettore[p->testa].stato =OCCUPATO;

    strcpy(stringa_shm, stringa);   // Copia della stringa
    shmdt(stringa_shm);
   
    p->testa=(p->testa+1)%DIM;
    p->count ++; 
    

    printf("Produzione completata: %s (%zu char, key=%x)\n", stringa_shm, lunghezza, chiave);
    signal_condition(&p->monitor, CV_CONSUMATORE);
    leave_monitor(&p->monitor);
}

void consuma(MonitorStreaming * p, char * stringa, size_t * lunghezza) {

    /* TBD: Completare il metodo, con la sincronizzazione */
    enter_monitor(&p->monitor);

    printf("Avvio consumazione...\n");
    if(p->count == 0){
        wait_condition(&p->monitor,CV_CONSUMATORE);
    }
    int i = 0 ; 
    while(p->vettore[i].stato ! = OCCUPATO){
        p->coda=(p->coda+1)%DIM;;
    }

    sleep(1);
    
    p->vettore[p->coda].stato=IN_USO;
    


    key_t chiave = p->vettore[p->coda].chiave;/* TBD */
    pid_t produttore = p->vettore[p->coda].produttore/* TBD */;
    *lunghezza = p->vettore[p->coda].dimensione/* TBD */;

    int shmid = shmget(chiave, *lunghezza +1 , IPC_CREAT | 0664);
    char * stringa_shm = shmat(shmid,NULL,0); /* TBD: Accedere alla stringa su shared memory */

    strcpy(stringa, stringa_shm);   // Copia della stringa
    shmdt(stringa_shm);

    p->vettore[p->coda].stato = LIBERO;
    p->coda=(p->coda+1)%DIM;
    p->count --; 

    printf("Consumazione completata: %s (%zu char, key=%x)\n", stringa, *lunghezza, chiave);
    signal_condition(&p->monitor,CV_PRODUTTORE);
    leave_monitor(&p->monitor);
}

void distruggi(MonitorStreaming * p) {

    /* TBD: Completare il metodo */

        for (int i = 0; i < DIM; i++) {
        if (p->vettore[i].stato != LIBERO) {
            int shmid = shmget(p->vettore[i].chiave, p->vettore[i].dimensione + 1, 0664);
            if (shmid != -1) {
                shmctl(shmid, IPC_RMID, NULL); // Rimuove la memoria condivisa
            }
        }
    }
    // Rimuove il monitor
    remove_monitor(&p->monitor);
}

