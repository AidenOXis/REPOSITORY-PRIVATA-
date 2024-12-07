#include "prodcons.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

void Produttore(MonitorStreaming *p);
void Consumatore(MonitorStreaming *p);

int main() {

    /* TBD: Creare un oggetto monitor, e avviare i processi produttore e consumatore */
    int id_shm=shmget(IPC_PRIVATE,sizeof(MonitorStreaming),IPC_CREAT|0664);
    MonitorStreaming * p = shmat(id_shm,NULL,0);
    

    inizializza(p);

    pid_t pid; 
    pid = fork();
    if(pid == 0) {
        // Processo figlio produttore
        srand(getpid());
        Produttore(p);
        exit(0);
    }
    
    // Creazione processo consumatore
    pid = fork();
    if(pid == 0) {
        // Processo figlio consumatore
        Consumatore(p);
        exit(0);
    }
    for(int i = 0; i < 2; i++) {
        wait(NULL);
    }
    
    // Pulizia risorse
    shmdt(p);
    shmctl(id_shm, IPC_RMID, NULL);
    distruggi(p);
    
    return 0;

}

void Produttore(MonitorStreaming *p) {

    char stringa[20];
    size_t lunghezza;

    char char_chiave = 'a';
    key_t chiave;

    for(int i=0; i<10; i++) {

        lunghezza = 1 + rand() % 20;

        for(int j=0; j<lunghezza-1; j++) {
            stringa[j] = 97 + (rand()%26);
        }

        stringa[lunghezza-1] = '\0';


        chiave = ftok(".",char_chiave);/* TBD */

        char_chiave = char_chiave + 1;


        /* TBD: Chiamare il metodo "produci()" del monitor */
        produci(p,stringa,lunghezza,chiave);

        sleep(1);
    }
}

void Consumatore(MonitorStreaming *p) {

    char stringa[20];
    size_t lunghezza;

    for(int i=0; i<10; i++) {

        /* TBD: Chiamare il metodo "produci()" del monitor */
        consuma(p,stringa,&lunghezza);

        sleep(1);
    }
}