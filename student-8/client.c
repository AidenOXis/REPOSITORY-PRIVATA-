#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "header_msg.h"

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

    /* TBD: Inviare un messaggio di "connect" */
    messaggio_connect msg_connect;
    msg_connect.type = RICHIESTA_CONNECT;
    msg_connect.pid = getpid();

    int ret = msgsnd(id_connect, &msg_connect, sizeof(messaggio_connect) - sizeof(long), 0);
    if (ret < 0) {
        perror("Errore invio connect");
        exit(1);
    }
    printf("[CLIENT] Invio richiesta di connessione (PID: %d)\n", getpid());

    /* TBD: Attendere la ricezione di un messaggio "ack" */
    printf("[CLIENT] Attesa messaggio ack...\n");
    messaggio_ack msg_ack;
    ret = msgrcv(id_ack, &msg_ack, sizeof(messaggio_ack) - sizeof(long), ACK_CONNECT, 0);
    if (ret < 0) {
        perror("Errore ricezione ack");
        exit(1);
    }
    printf("[CLIENT %d] Connesso. Code richieste: %d, risposte: %d\n",
           getpid(), msg_ack.id_coda_richiesta, msg_ack.id_coda_risposta);

    /* TBD: Ottenere gli ID delle code dedicate */
    int id_coda_richieste = msg_ack.id_coda_richiesta;
    int id_coda_risposte = msg_ack.id_coda_risposta;

    for (int i = 0; i < 3; i++) {

        printf("[CLIENT %d] Invio richiesta produzione\n", getpid());

        int valore = rand() % 10;

        /* TBD: Inviare un messaggio di richiesta di tipo "PRODUZIONE", con il valore */
        messaggio_richiesta msg_req;
        msg_req.type = RICHIESTA_PRODUZIONE;
        msg_req.valore = valore;

        ret = msgsnd(id_coda_richieste, &msg_req, sizeof(messaggio_richiesta) - sizeof(long), 0);
        if (ret < 0) {
            perror("Errore invio richiesta produzione");
            exit(1);
        }

        /* TBD: Attendere un messaggio di risposta di tipo "PRODUZIONE" */
        messaggio_risposta msg_risp;
        ret = msgrcv(id_coda_risposte, &msg_risp, sizeof(messaggio_risposta) - sizeof(long), RISPOSTA_PRODUZIONE, 0);
        if (ret < 0) {
            perror("Errore ricezione risposta produzione");
            exit(1);
        }

        printf("[CLIENT %d] Valore prodotto: %d\n", getpid(), valore);
    }

    for (int i = 0; i < 3; i++) {

        printf("[CLIENT %d] Invio richiesta consumazione\n", getpid());

        /* TBD: Inviare un messaggio di richiesta di tipo "CONSUMAZIONE", con nessun valore */
        messaggio_richiesta msg_req;
        msg_req.type = RICHIESTA_CONSUMAZIONE;

        ret = msgsnd(id_coda_richieste, &msg_req, sizeof(messaggio_richiesta) - sizeof(long), 0);
        if (ret < 0) {
            perror("Errore invio richiesta consumazione");
            exit(1);
        }

        /* TBD: Attendere un messaggio di risposta di tipo "CONSUMAZIONE", ed estrapolare il valore */
        messaggio_risposta msg_risp;
        ret = msgrcv(id_coda_risposte, &msg_risp, sizeof(messaggio_risposta) - sizeof(long), RISPOSTA_CONSUMAZIONE, 0);
        if (ret < 0) {
            perror("Errore ricezione risposta consumazione");
            exit(1);
        }

        int valore = msg_risp.valore;
        printf("[CLIENT %d] Valore consumato: %d\n", getpid(), valore);
    }

    /* TBD: Rimuovere le due code di richieste e risposte dedicate alla connessione */
    if (msgctl(id_coda_richieste, IPC_RMID, NULL) < 0) {
        perror("Errore rimozione coda richieste");
    }

    if (msgctl(id_coda_risposte, IPC_RMID, NULL) < 0) {
        perror("Errore rimozione coda risposte");
    }

    return 0;
}