#ifndef _HEADER_MSG_H_
#define _HEADER_MSG_H_

#include <sys/types.h>

/* TBD: Definire i valori per i campi-tipo dei messaggi */
#define RICHIESTA_CONNECT 1
#define ACK_CONNECT 2 
#define RICHIESTA_PRODUZIONE 3
#define RICHIESTA_CONSUMAZIONE 4
#define RISPOSTA_PRODUZIONE 5
#define RISPOSTA_CONSUMAZIONE 6

typedef struct {
    
    /* TBD: Definire la struttura del messaggio,
            includendo un campo per inserire il PID del mittente
     */
    long type; 
    long pid; 

} messaggio_connect;

typedef struct {

    /* TBD: Definire la struttura del messaggio,
            includendo un campo per inserire l'ID della coda di richieste,
            e un campo per inserire l'ID della coda di risposte.
     */
    long type; 
    int id_coda_richiesta;
    int id_coda_risposta;


} messaggio_ack;

typedef struct {
    
    /* TBD: Definire la struttura del messaggio,
            includendo un campo per inserire un valore intero.
    */
   long type; 
   int valore; 

} messaggio_richiesta;

typedef struct {

    /* TBD: Definire la struttura del messaggio,
            includendo un campo per inserire un valore intero.
    */
   long type; 
   int valore; 

} messaggio_risposta;

#endif