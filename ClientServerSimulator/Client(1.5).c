/**
 * @file ClientAndServer.c
 * @author Alessandro Ferrante (https://alessandroferrante.net)
 * @brief 
 * @version 1.5
 * @date 2024-03-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/*
 ! Work in progress...
 ! Questa è un'atra versione del Client, per trovare una giusta soluzione 
 ! alla concorenza al file chat.txt e alla terminazione di un nuovo client che si vorrà connettere, perchè trova stato 201.
 ! This is another version of the Client, to find the right solution
 ! to the concurrence of the chat.txt file and the termination of a new client that you want to connect, because it finds status 201.
 *
 *
 | Questo codice simula un Client e il suo funzionamento,
 | La funzione client appena si avvia controlla lo stato del server che 
 | se conferma con status code 100, il client consente ti secegliere 
 | quale richiesta inviare, e successivamente la comunica scrivendo sul un file in modalità append(a+),
 | non appena il file viene aggornato con la nuova richiesta, il server la legge.
 | Il server è in ascolto quasi continuo per poter controllare se la lunghezza del file è cambiata, 
 | dando cosí al client la possibilità di poter leggere e scrivere sullo stesso file.
 | A seconda delle richieste il server inviaerà codici di stato differenti.

 | This code simulates a Client and its operation,
 | The client function checks the status of the server as soon as it starts
 | if you confirm with status code 100, the client allows you to choose
 | which request to send, and subsequently communicates it by writing to a file in append(a+) mode,
 | as soon as the file is updated with the new request, the server reads it.
 | The server is listening almost continuously to be able to check if the length of the file has changed,
 | thus giving the client the possibility of being able to read and write to the same file.
 | Depending on the requests, the server will send different status codes.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void startClient();

typedef struct Clientrequest{
    int action;
    char username[20];
    char password[30];
}request;

typedef struct ServerResponse{
    int status;
    char id[10];
    int UserId;
    char msg[100];
}response;

typedef struct Account{
    char username[20];
    char password[30];
}account;

//-Client
void Client(int LocalUserid){ 
    response server;
    FILE *freadchat = NULL;
    if((freadchat=fopen("chat.txt", "r"))== NULL){
        printf("%s\n", "Client 1 Error 404!");
        exit(1);
    }
    //? Memorizza lo stato del server
    while(!feof(freadchat)){
        fscanf(freadchat, "%d %s %d %99[^\n]s", &server.status, server.id, &server.UserId ,server.msg);
    }
    fclose(freadchat);
    
    //? apertura file in modalità scrittura
    FILE *fwritechat= NULL;
        if((fwritechat=fopen("chat.txt", "a+"))== NULL){
            printf("%s\n", "Client 2 Error 404!");
            exit(1);
    }
    request req;
    //? Verifica status code server
    switch(server.status){
        case 100: 
        //? Response con status 100 fa partire la richiesta del client;
            printf("%s\n", "Server in attesa della risposta");
            printf("%s\n", "Client running");
            
            printf("%s\n","--| Creation of the request to send to the Server |--");
            printf("%s\n","Insert 0 to login and 1 to register");
            scanf("%d", &req.action);
            printf("%s", "Insert username: ");
            scanf("%s", req.username);
            printf("%s", "Insert password: ");
            scanf("%s", req.password);
            
            fprintf(fwritechat, "%d %s %s %d\n", req.action, req.username, req.password, LocalUserid);

            fclose(fwritechat);
            break;
        case 200: 
            printf("%s\n", "OK richiesta andata a buon fine");
            fclose(fwritechat);
            break;
        case 201: 
            printf("%s\n", "StatusCode 201 The Server satisfied the request, logged in");
            fclose(fwritechat);           
            exit(EXIT_SUCCESS); //! END ( TERMINATOR ;) ) 
            break;
        case 204:
            printf("%s\n", "StatusCode 204 The Server processed but did not return access");
            printf("%s", "Do you want to login? Enter 'y' to enter or any key to exit: ");
            char c[1];
            scanf("%s", c);
            if(strcmp(c, "y")==0){
                printf("%s", "Insert username: ");
                scanf("%s", req.username);
                printf("%s", "Insert password: ");
                scanf("%s", req.password);
                    
                fprintf(fwritechat, "%d %s %s %d\n", 0, req.username, req.password, LocalUserid);
                
                fclose(fwritechat);
            }
            fclose(fwritechat);
            break;
        case 401:
            printf("%s\n", "The Server did not allow access, user not registered, try to access again or register");
            printf("%s\n", "Client running");

            printf("%s\n","--| Creation of the request to send to the Server |--");
            printf("%s\n","Insert 0 to login and 1 to register");
            scanf("%d", &req.action);
            printf("%s", "Insert username: ");
            scanf("%s", req.username);
            printf("%s", "Insert password: ");
            scanf("%s", req.password);
            
            fprintf(fwritechat, "%d %s %s %d\n", req.action, req.username, req.password, LocalUserid);
            fclose(fwritechat);
            break;
        case 403: 
            printf("%s\n", "Il Server non ha consetito la registrazione, nome utente non disponibile, riprova");
            printf("%s\n", "Client running");

            printf("%s\n","--| Creation of the request to send to the Server |--");
            printf("%s\n","Insert 0 to login and 1 to register");
            scanf("%d", &req.action);
            printf("%s", "Insert username: ");
            scanf("%s", req.username);
            printf("%s", "Insert password: ");
            scanf("%s", req.password);
    
            
            fprintf(fwritechat, "%d %s %s %d\n", req.action, req.username, req.password, LocalUserid);
            
            fclose(fwritechat);
            break;
        case 404: //? nel caso in cui il file accounts non è accessibile
            printf("%s%s\n", server.msg, ", riprova");
            printf("%s\n", "Client running");

            printf("%s\n","--| Creation of the request to send to the Server |--");
            printf("%s\n","Insert 0 to login and 1 to register");
            scanf("%d", &req.action);
            printf("%s", "Insert username: ");
            scanf("%s", req.username);
            printf("%s", "Insert password: ");
            scanf("%s", req.password);

            
            fprintf(fwritechat, "%d %s %s %d\n", req.action, req.username, req.password, LocalUserid);
            
            fclose(fwritechat);
            break;
    }  
    fclose(fwritechat);
}

void startClient(){
    int LocalUserid = 0;
    printf("%s %d\n", "Starting Client...", LocalUserid);
    while(1){
        Client(LocalUserid);
        sleep(1);
    }
}

int main(int argc, char *argv){
    startClient();
    return 0;
}