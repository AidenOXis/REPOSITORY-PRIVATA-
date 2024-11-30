#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {

    /* TBD: Creare la coda di messaggi per
            i messaggi di tipo "connect"
     */
    key_t connect_key = ftok(".",'a'); 
    int id_connect = msgget(connect_key,IPC_CREAT | 0666);
    if(id_connect < 0 ){
        perror("Errore creazione dei messaggi"); 
        exit(1); 
    }

    /* TBD: Creare la coda di messaggi per
            i messaggi di tipo "acknowledge"
     */
    key_t ack_key = ftok(".",'b'); 
    int id_ack= msgget(ack_key,IPC_CREAT | 0666); 
    if(id_ack<0){
        perror("Errore creazione coda messaggi"); 
        exit(1); 
    }


    /* TBD: Creare un processo figlio,
            e fargli eseguire il programma "server"
     */
    printf("[%d] Avvio server \n", getpid()); 
    pid_t pid = fork(); 
    if(pid == 0 ){
        execl("./server","server",NULL); 
        perror("Errore excec server"); 
        exit(1); 
    }


    /* TBD: Creare due processi figli,
            e fargli eseguire il programma "client"
     */
    for(int i = 0; i<2; i++){
        printf("[%d] Avvio client \n",getpid()); 
        pid_t pid = fork(); 
        if(pid == 0 ){
                execl("./client","client",NULL);
                perror("Errore excec client"); 
                exit(1); 
        }
    }
    printf("[%d] In attesa di terminazione ... \n", getpid()); 
    /* TBD: Attendere la terminazione dei 3 processi figli */
    for(int i=0; i<3;i++){
        wait(NULL); 
    }
    printf("[%d] Deallocazione code\n",getpid()); 
    /* TBD: De-allocare le due code di messaggi */
    msgctl(id_connect,IPC_RMID,NULL); 
    msgctl(id_ack,IPC_RMID,NULL); 
    return 0 ; 

}