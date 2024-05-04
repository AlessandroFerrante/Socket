/**
 * @file Client.c
 * @author Alessandro Ferrante (https://alessandroferrante.net)
 * @brief 
 * @version 1.0
 * @date 2024-03-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/*
 | Questo codice simula un Client e il suo funzionamento,
 | La funzione client appena si avvia controlla lo stato del server che 
 | se conferma con status code 100, il client consente ti secegliere 
 | quale richiesta inviare, e successivamente la comunica scrivendo sul un file in modalità append(a+),
 | non appena il file viene aggornato con la nuova richiesta, il server la legge.
 | Il server è in ascolto quasi continuo per poter controllare se la lunghezza del file è cambiata, 
 | dando cosí al client la possibilità di poter leggere e scrivere sullo stesso file.
 | A seconda delle richieste il server inviaerà codici di stato differenti.
 *
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
#include <unistd.h>

void startClient();

typedef struct Clientrequest{
    int action;
    char id[20];
    char password[30];
}request;

typedef struct ServerResponse{
    int status;
    char id[10];
    char msg[100];
}response;

typedef struct Account{
    char id[20];
    char password[30];
}account;

//-Client
void Client(){ 
    response server;
    FILE *freadchat = NULL;
    if((freadchat=fopen("chat.txt", "r"))== NULL){
        printf("%s\n", "Client Error 404!");
        exit(1);
    }
    //? Stores the server state
    while(!feof(freadchat)){
        fscanf(freadchat, "%d %s %99[^\n]s", &server.status, server.id, server.msg);
    }
    fclose(freadchat);
    
    //? Opening file in write mode
    FILE *fwritechat= NULL;
        if((fwritechat=fopen("chat.txt", "a+"))== NULL){
            printf("%s\n", "Client Error 404!");
            exit(1);
    }
    request req;
    //? Check status code server
    switch(server.status){
        case 100: 
        //? Response with status 100 starts the client request;
            printf("%s\n", "Server waiting for response");
            printf("%s\n", "Client running");
            
            printf("%s\n","--| Creation of the request to send to the Server |--");
            printf("%s\n","Insert 0 to login and 1 to register");
            scanf("%d", &req.action);
            printf("%s", "Insert id: ");
            scanf("%s", req.id);
            printf("%s", "Insert password: ");
            scanf("%s", req.password);
                
            
            fprintf(fwritechat, "%d %s %s\n", req.action, req.id, req.password);

            fclose(fwritechat);
            break;
        case 200: 
            printf("%s\n", "OK request successful");
            fclose(fwritechat);
            break;
        case 201: 
            printf("%s\n", "StatusCode 201 The Server satisfied the request, logged in");
            fclose(fwritechat);
            exit(1);
            break;
        case 204:
            printf("%s\n", "StatusCode 204 The Server processed but did not return access");
            printf("%s", "Do you want to login? Enter 'y' to enter or any key to exit: ");
            char c[1];
            scanf("%s", c);
            if(strcmp(c, "y")==0){
                printf("%s", "Insert id: ");
                scanf("%s", req.id);
                printf("%s", "Insert password: ");
                scanf("%s", req.password);
                    
                fprintf(fwritechat, "%d %s %s\n", 0, req.id, req.password);
                
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
            printf("%s", "Insert id: ");
            scanf("%s", req.id);
            printf("%s", "Insert password: ");
            scanf("%s", req.password);
            
            fprintf(fwritechat, "%d %s %s\n", req.action, req.id, req.password);
            
            fclose(fwritechat);
            break;
        case 403: 
            printf("%s\n", "The Server did not allow registration, username not available, try againa");
            printf("%s\n", "Client running");

            printf("%s\n","--| Creation of the request to send to the Server |--");
            printf("%s\n","Insert 0 to login and 1 to register");
            scanf("%d", &req.action);
            printf("%s", "Insert id: ");
            scanf("%s", req.id);
            printf("%s", "Insert password: ");
            scanf("%s", req.password);
    
            
            fprintf(fwritechat, "%d %s %s\n", req.action, req.id, req.password);
            
            fclose(fwritechat);
            break;
        case 404: //? in case the accounts file is not accessible
            printf("%s%s\n", server.msg, ", try again");
            printf("%s\n", "Client running");

            printf("%s\n","--| Creation of the request to send to the Server |--");
            printf("%s\n","Insert 0 to login and 1 to register");
            scanf("%d", &req.action);
            printf("%s", "Insert id: ");
            scanf("%s", req.id);
            printf("%s", "Insert password: ");
            scanf("%s", req.password);

            
            fprintf(fwritechat, "%d %s %s\n", req.action, req.id, req.password);
            
            fclose(fwritechat);
            break;
    }  
    fclose(fwritechat);
}

void startClient(){
    printf("%s\n", "Starting Client...");
    while(1){
        Client();
        sleep(1);
    }
}

int main(int argc, char *argv[]){
    startClient();
    return 0;
}