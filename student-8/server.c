#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "header_msg.h"
#include "header_monitor.h"


void * thread_worker(void *);

int main() {

    srand(getpid());


    /* TBD: Ottenere le due code di messaggi "connect" ed "ack"
            (le stesse create nel programma "start")
     */
    key_t connect_key = ftok(".", 'a');
    int id_connect = msgget(connect_key, 0666);
    if (id_connect < 0) {
        perror("Errore apertura coda connect");
        exit(1);
    }

    key_t ack_key = ftok(".", 'b');
    int id_ack = msgget(ack_key, 0666);
    if (id_ack < 0) {
        perror("Errore apertura coda ack");
        exit(1);
    }


    MonitorPC *p = /* TBD: Allocare e inizializzare un oggetto monitor */;



    pthread_t worker[2];


    // Ripete per 2 client

    for(int i=0; i<2; i++) {

        /* TBD: Attendere un messaggio "connect" */

        pid_t pid = /* TBD: Estrarre il PID del client dal messaggio "connect" */;
        

        int id_coda_richieste = /* TBD: Creare una coda di richieste, dedicata alla nuova connessione */

        int id_coda_risposte = /* TBD: Creare una coda di risposte, dedicata alla nuova connessione */



        /* TBD: Inviare un messaggio "ack", contenente gli ID delle code di richieste e risposte */



        printf("[SERVER] Connesso client %d\n", pid);


        /* TBD: Creare un thread worker, che esegua la funzione
                "thread_worker()".

                Passare come input al thread
                le seguenti variabili:
                - id coda richieste
                - id coda risposte
                - oggetto monitor
                - PID del client
        */

    }

    for(int i=0; i<2; i++) {

        /* TBD: Attendere la terminazione dei thread */
    }


    /* TBD: Deallocare l'oggetto monitor */

}


void * thread_worker(void * arg) {

    /* TBD: Completare il passaggio dei parametri */

    int id_coda_richieste = /* TBD */;
    int id_coda_risposte = /* TBD */;
    MonitorPC *p = /* TBD */;
    pid_t pid = /* TBD */;


    // Ogni worker serve 6 richieste, e poi termina

    for(int i=0; i<6; i++) {

        printf("[SERVER] In attesa di richiesta dal client %d\n", pid);


        /* TBD: Attendere un messaggio dalla coda delle richieste */

        if(/* TBD: Richiesta di tipo "PRODUZIONE" */) {

            printf("[SERVER] Richiesta di produzione dal client %d\n", pid);


            int valore = /* TBD: Estrarre il valore dal messaggio di richeista*/;

            /* TBD: Chiamare il metodo produci(), passando in ingresso "valore" */

            printf("[SERVER] Valore prodotto: %d (client %d)\n", valore, pid);



            /* TBD: Inviare un messaggio di risposta di tipo "PRODUZIONE", con valore 0 */

            printf("[SERVER] Risposta inviata al client %d\n", pid);

        }
        else if(/* TBD: Richiesta di tipo "CONSUMAZIONE" */) {

            printf("[SERVER] Richiesta di consumazione dal client %d\n", pid);

            int valore = /* TBD: Chiamare il metodo consuma() */;

            printf("[SERVER] Valore consumato: %d (client %d)\n", valore, pid);



            /* TBD: Inviare un messaggio di risposta di tipo "CONSUMAZIONE", con il valore consumato */

            printf("[SERVER] Risposta inviata al client %d\n", pid);

        }
    }


    pthread_exit(NULL);
}