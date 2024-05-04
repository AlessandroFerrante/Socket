/**
 * @file Server.c
 * @author Alessandro Ferrante (https://alessandroferrante.netlify.app)
 * @brief 
 * @version 1.0
 * @date 2024-03-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/*
 _ This code simulates a Server and its operation,
 _ The Server function checks in a loop whether the file is updated,
 _ in this way it allows you to simulate a listening Server,
 _ The function allows you to respond to a customer's requests
 _ with status codes, interpreted by the Client process itself.
 * ---------------------------------------------------------------
 | Questo codice simula un Server e il suo funzionamento, 
 | La funzione server controlla in loop se il file viene aggiornato,
 | in questo modo permette di simulare un Server in ascolto, 
 | il la funzione permette di rispondere alle richieste di un Client 
 | con codici di stato, interpretati dal processo Client stesso.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct Clientrequest{
    int UserId;
    int action;
    char username[20];
    char password[30];
}request;

typedef struct ServerResponse{
    int status;
    char id[10];
    char msg[100];
}response;

typedef struct Account{
    char username[20];
    char password[30];
}account;


//-Server
void Server(long *lastPosition){
    // FILE chat.txt
    FILE *freadchat= NULL;
    if((freadchat=fopen("chat.txt", "r"))== NULL){
        printf("%s\n", "Server Error 404!");
        exit(1);
    }

    fseek(freadchat, 0, SEEK_END);
    long newPosition = ftell(freadchat); //? new file length
    
    //? I check if the file has been updated
    if(newPosition > *lastPosition){    
        static int Nreq = 0;
        printf("%s %d\n", "Request...", Nreq++);
        
        request req;
        fseek(freadchat, *lastPosition, SEEK_SET);
        fscanf(freadchat, "%d %s %s %d\n", &req.action, req.username, req.password, &req.UserId);
        *lastPosition = ftell(freadchat);
        fclose(freadchat);

        FILE *fwritechat = NULL;
        if((fwritechat=fopen("chat.txt", "a+"))==NULL){
            printf("%s\n", "Server Error 404!");
            exit(1);
        }
        
        // FILE accounts.txt
        FILE *f= NULL;
        if((f=fopen("accounts.txt", "a+"))==NULL){
            fprintf(fwritechat, "%d %s %d %s\n", 404, "Server", req.UserId, "Error404, apertura file `accounts.txt` fallita");
        }
        else{
            account a;
            int aa = 1;
            switch(req.action){
                case 0://? access request
                    while(!feof(f)){
                        fscanf(f, "%s %s", a.username, a.password);
                        if(strcmp(req.username, a.username)==0 && strcmp(req.password, a.password)==0){
                            fprintf(fwritechat, "%d %s %d %s\n", 201, "Server", req.UserId, "Access allowed!");
                            fseek(fwritechat, 0, SEEK_END);
                            *lastPosition = ftell(fwritechat);
                            fclose(fwritechat);
                            break;
                        }
                    }
                    fprintf(fwritechat, "%d %s %d %s\n", 401, "Server", req.UserId, "Accesso non efettuato!");

                    fclose(fwritechat);
                    fclose(f);
                    break;
                case 1: //? registration request
                    while(!feof(f)){
                        fscanf(f, "%s %s", a.username, a.password);
                        if(strcmp(req.username, a.username)==0){
                            fprintf(fwritechat, "%d %s %d %s\n", 403, "Server", req.UserId, "Registration not carried out!");
                            fseek(fwritechat, 0, SEEK_END);
                            *lastPosition = ftell(fwritechat);
                            fclose(fwritechat); 
                            fclose(f);
                            break;
                        }
                    }
                    fseek(f, 0, SEEK_END);
                    fprintf(f, "%s %s\n", req.username, req.password);
                    fprintf(fwritechat, "%d %s %d %s\n", 204, "Server", req.UserId, "Registration completed!");
    
                    fclose(fwritechat);
                    fclose(f);
                    break;          
                default:
                    fseek(fwritechat, 0, SEEK_END);
                    *lastPosition = ftell(fwritechat);
                    fclose(fwritechat);
                    fclose(f);
                    break;     
            }
        }
        fclose(fwritechat);
    }
    else{
        fclose(freadchat);
    }
}

void startServer(){
    printf("%s\n", "Starting server...");
    
    FILE *f= NULL;
    if((f=fopen("chat.txt", "w"))== NULL){
        printf("%s\n", "Server Error 404!");
        exit(1);
    }
    //? writes to the file that the server is waiting for the request with status 100
    fprintf(f, "%d %s %d %s\n", 100, "startServer", 0, "Waiting for new requests");
    long lastPosition = ftell(f); //? for the initial length of the file
    fclose(f);
    printf("%s\n", "Server running...");
    while(1){
        Server(&lastPosition);
        sleep(1);
   }
}

int main(int argc, char * argv[]){
    startServer();

    return 0;
}