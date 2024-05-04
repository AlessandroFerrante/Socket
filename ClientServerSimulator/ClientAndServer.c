/**
 * @file ClientAndServer.c
 * @author Alessandro Ferrante
 * @brief 
 * @version 0.1
 * @date 2024-03-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void Client();
void Server();
void startClient();
void startServer();

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
        printf("%s\n", "Client 1 Error 404!");
        exit(1);
    }
    //? Memorizza lo stato del server
    fscanf(freadchat, "%d %s %99[^\n]s", &server.status, server.id, server.msg);
    fclose(freadchat);
    
    //? apertura file in modalità scrittura
    FILE *fwritechat= NULL;
        if((fwritechat=fopen("chat.txt", "w"))== NULL){
            printf("%s\n", "Client 2 Error 404!");
            exit(1);
    }
    fseek(fwritechat, 0, SEEK_SET);
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
            printf("%s", "Insert id: ");
            scanf("%s", req.id);
            printf("%s", "Insert password: ");
            scanf("%s", req.password);

            fprintf(fwritechat, "%d %s %s", req.action, req.id, req.password);

            fclose(fwritechat);
            break;
        case 200: 
            printf("%s\n", "OK richiesta andata a buon fine");
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

                fprintf(fwritechat, "%d %s %s", 0, req.id, req.password);
                
                fclose(fwritechat);
                Server();
            }
            fclose(fwritechat);
            exit(1);
            break;
        case 401:
            printf("%s\n", "Il Server non ha consetito l'accesso, utente non registrato, riprova ad accedere o registrati");
            printf("%s\n", "Client running");

            printf("%s\n","--| Creation of the request to send to the Server |--");
            printf("%s\n","Insert 0 to login and 1 to register");
            scanf("%d", &req.action);
            printf("%s", "Insert id: ");
            scanf("%s", req.id);
            printf("%s", "Insert password: ");
            scanf("%s", req.password);

            fprintf(fwritechat, "%d %s %s", req.action, req.id, req.password);
            
            fclose(fwritechat);
            break;
        case 403: 
            printf("%s\n", "Il Server non ha consetito la registrazione, nome utente non disponibile, riprova");
            printf("%s\n", "Client running");

            printf("%s\n","--| Creation of the request to send to the Server |--");
            printf("%s\n","Insert 0 to login and 1 to register");
            scanf("%d", &req.action);
            printf("%s", "Insert id: ");
            scanf("%s", req.id);
            printf("%s", "Insert password: ");
            scanf("%s", req.password);

            fprintf(fwritechat, "%d %s %s", req.action, req.id, req.password);
            
            fclose(fwritechat);
            break;
        case 404: //? nel caso in cui il file accounts non è accessibile
            printf("%s%s\n", server.msg, ", riprova");
            printf("%s\n", "Client running");

            printf("%s\n","--| Creation of the request to send to the Server |--");
            printf("%s\n","Insert 0 to login and 1 to register");
            scanf("%d", &req.action);
            printf("%s", "Insert id: ");
            scanf("%s", req.id);
            printf("%s", "Insert password: ");
            scanf("%s", req.password);

            fprintf(fwritechat, "%d %s %s", req.action, req.id, req.password);
            
            fclose(fwritechat);
            break;
        /*default: //! case 404 il server non h scritto su file
            printf("%s\n", "Il Server non ha potuto portare a termine la richiesta");
            fclose(fwritechat);
            break;*/
    }  
    fclose(fwritechat);
    Server();
}


//-Server
void Server(){
    // FILE chat.txt
    FILE *freadchat= NULL;
    if((freadchat=fopen("chat.txt", "r"))== NULL){
        printf("%s\n", "Server Error 404!");
        exit(1);
    }
    printf("\n%s\n", "Starting server...");
    request req;
    fseek(freadchat, 0, SEEK_END);
    fscanf(freadchat, "%d %s %s\n", &req.action, req.id, req.password);
    fclose(freadchat);

    FILE *fwritechat = NULL;
    if((fwritechat=fopen("chat.txt", "w"))==NULL){
        printf("%s\n", "Server Error 404!");
        exit(1);
    }
    fseek(fwritechat, 0, SEEK_SET);

    response resp;

    // FILE accounts.txt
    FILE *f= NULL;
    if((f=fopen("accounts.txt", "a+"))==NULL){
        fprintf(fwritechat, "%d %s %s", 404, "Server", "Error404, apertura file `accounts.txt` fallita");
    }
    else{
        account a;
        switch(req.action){
            case 0://? richiesta di accesso
                while(!feof(f)){
                    fscanf(f, "%s %s", a.id, a.password);
                    if(strcmp(req.id, a.id)==0 && strcmp(req.password, a.password)==0){
                        fprintf(fwritechat, "%d %s %s", 201, "Server", "Accesso efettuato!");
                        fclose(fwritechat);
                        break;
                    }
                }
                fprintf(fwritechat, "%d %s %s", 401, "Server", "Accesso non efettuato!");
                fclose(fwritechat);
                fclose(f);
                break;
            case 1: //? richiesta di registrazione
                while(!feof(f)){
                    fscanf(f, "%s %s", a.id, a.password);
                    if(strcmp(req.id, a.id)==0){
                        fprintf(fwritechat, "%d %s %s", 403, "Server", "Registrazione non efettuata!");
                        fclose(fwritechat); 
                        fclose(f);
                        break;
                    }
                }
                fseek(f, 0, SEEK_END);
                fprintf(f, "%s %s\n", req.id, req.password);
                fprintf(fwritechat, "%d %s %s", 204, "Server", "Registrazione efettuata!");
                fclose(fwritechat);
                fclose(f);
                break;
            default:
                fclose(fwritechat);
                fclose(f);
                break;                
        }
    }
    fclose(fwritechat);
    Client();
}

void startClient(){
    printf("%s\n", "Starting Client...");
    Client();
}

void startServer(){
    printf("%s\n", "Starting server...");
    FILE *f= NULL;
    if((f=fopen("chat.txt", "w"))== NULL){
        printf("%s\n", "Server Error 404!");
        exit(1);
    }
    //? scrive sul file che il server è in attesa della richiesta con status 100
    fprintf(f, "%d %s %s", 100, "startServer", "");
    printf("%s\n", "Server running");
    fclose(f);
}

int main(int argc, char * argv){
    startServer();
    startClient();

    return 0;
}