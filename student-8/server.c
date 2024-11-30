#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>

#include "header_msg.h"
#include "header_monitor.h"

// Prototipo della funzione worker
void *thread_worker(void *);

int main() {
    srand(getpid());

    /* TBD: Ottenere le due code di messaggi "connect" ed "ack" */
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

    /* Allocare e inizializzare un oggetto monitor */
    MonitorPC *p = (MonitorPC *)malloc(sizeof(MonitorPC));
    if (p == NULL) {
        perror("Errore allocazione monitor");
        exit(1);
    }
    init_monitorpc(p);

    pthread_t worker[2];

    // Ripete per 2 client
    for (int i = 0; i < 2; i++) {
        /* Attendere un messaggio "connect" */
        messaggio_connect msg_connect;
        if (msgrcv(id_connect, &msg_connect, sizeof(messaggio_connect) - sizeof(long), 0, 0) < 0) {
            perror("Errore ricezione messaggio connect");
            exit(1);
        }
        pid_t pid = msg_connect.pid; // Estrarre il PID del client

        /* Creare una coda di richieste e una di risposte per la connessione */
        int id_coda_richieste = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
        if (id_coda_richieste < 0) {
            perror("Errore creazione coda richieste");
            exit(1);
        }

        int id_coda_risposte = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
        if (id_coda_risposte < 0) {
            perror("Errore creazione coda risposte");
            exit(1);
        }

        /* Inviare un messaggio "ack" con gli ID delle code */
        messaggio_ack msg_ack;
        msg_ack.type = ACK_CONNECT;
        msg_ack.id_coda_richiesta = id_coda_richieste;
        msg_ack.id_coda_risposta = id_coda_risposte;
        if (msgsnd(id_ack, &msg_ack, sizeof(messaggio_ack) - sizeof(long), 0) < 0) {
            perror("Errore invio messaggio ack");
            exit(1);
        }

        printf("[SERVER] Connesso client %d\n", pid);

        /* Creare un thread worker */
        typedef struct {
            int id_coda_richieste;
            int id_coda_risposte;
            MonitorPC *monitor;
            pid_t pid;
        } thread_args;

        thread_args *args = (thread_args *)malloc(sizeof(thread_args));
        args->id_coda_richieste = id_coda_richieste;
        args->id_coda_risposte = id_coda_risposte;
        args->monitor = p;
        args->pid = pid;

        if (pthread_create(&worker[i], NULL, thread_worker, (void *)args) != 0) {
            perror("Errore creazione thread worker");
            exit(1);
        }
    }

    // Attendere la terminazione dei thread
    for (int i = 0; i < 2; i++) {
        pthread_join(worker[i], NULL);
    }

    /* Deallocare l'oggetto monitor */
    remove_monitorpc(p);
    free(p);

    return 0;
}

void *thread_worker(void *arg) {
    /* Estrarre i parametri passati */
    typedef struct {
        int id_coda_richieste;
        int id_coda_risposte;
        MonitorPC *monitor;
        pid_t pid;
    } thread_args;

    thread_args *args = (thread_args *)arg;
    int id_coda_richieste = args->id_coda_richieste;
    int id_coda_risposte = args->id_coda_risposte;
    MonitorPC *p = args->monitor;
    pid_t pid = args->pid;
    free(args);

    // Ogni worker serve 6 richieste, e poi termina
    for (int i = 0; i < 6; i++) {
        printf("[SERVER] In attesa di richiesta dal client %d\n", pid);

        /* Attendere un messaggio dalla coda delle richieste */
        messaggio_richiesta msg_req;
        if (msgrcv(id_coda_richieste, &msg_req, sizeof(messaggio_richiesta) - sizeof(long), 0, 0) < 0) {
            perror("Errore ricezione richiesta");
            pthread_exit(NULL);
        }

        if (msg_req.type == RICHIESTA_PRODUZIONE) {
            printf("[SERVER] Richiesta di produzione dal client %d\n", pid);

            /* Estrarre il valore dal messaggio e chiamare produci */
            int valore = msg_req.valore;
            produci(p, valore);

            printf("[SERVER] Valore prodotto: %d (client %d)\n", valore, pid);

            /* Inviare una risposta */
            messaggio_risposta msg_risp = {RISPOSTA_PRODUZIONE, 0};
            if (msgsnd(id_coda_risposte, &msg_risp, sizeof(messaggio_risposta) - sizeof(long), 0) < 0) {
                perror("Errore invio risposta produzione");
            }
        } else if (msg_req.type == RICHIESTA_CONSUMAZIONE) {
            printf("[SERVER] Richiesta di consumazione dal client %d\n", pid);

            /* Chiamare consuma */
            int valore = consuma(p);

            printf("[SERVER] Valore consumato: %d (client %d)\n", valore, pid);

            /* Inviare una risposta */
            messaggio_risposta msg_risp = {RISPOSTA_CONSUMAZIONE, valore};
            if (msgsnd(id_coda_risposte, &msg_risp, sizeof(messaggio_risposta) - sizeof(long), 0) < 0) {
                perror("Errore invio risposta consumazione");
            }
        }
    }

    printf("[SERVER] Worker per client %d terminato\n", pid);
    pthread_exit(NULL);
}
