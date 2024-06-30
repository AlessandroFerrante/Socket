/**
 * @file client.c
 * @author Alessandro Ferrante github@alessandroferrante.net
 * @brief 
 * EN| This code implements a client that connects to a server via UDP socket to play the hangman game.
 * The client connects to the server, receives the current state of the game, sends the letters it thinks are in the word
 * to guess, and receives updates from the game state until he wins or loses.
 * ---
 * IT| Questo codice implementa un client che si connette a un server tramite socket UDP per giocare al gioco dell'impiccato.
 * Il client si connette al server, riceve lo stato attuale del gioco, invia le lettere che pensa siano nella parola
 * da indovinare, e riceve aggiornamenti dallo stato del gioco fino a che non vince o perde.
 * @version 0.1
 * @date 2024-04-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1000

int main(int argc, char **argv){
    
    // socket descriptor
    int sockfd, n;
    // remote address (server)
    struct sockaddr_in remote_addr;
    // local and remote address (server and client)
    socklen_t len =  sizeof(struct sockaddr_in);
    char sendline[BUFFER_SIZE];
    char recvline[BUFFER_SIZE];
   

    /*
    if(argc!=3) {
        printf("Errore Inserisci IP e port\n");
        return -1;
    }*/

    sockfd =  socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0) {
        printf("Errore nell'apertura di socket\n");
        return -1;
    }

    // fill memory with a constant byte, set all bytes to 0
    memset(&remote_addr, 0, len);
    // IPv4 address family
    remote_addr.sin_family = AF_INET;
    // convert IPv4 and IPv6 addresses from text to binary form
    inet_pton(AF_INET, "0.0.0.0", &(remote_addr.sin_addr));
    // convert the port number from host byte order to network byte order
    remote_addr.sin_port = htons(atoi("8080"));
    
    fgets(sendline, 1000, stdin);

    sendto(sockfd, sendline, strlen(sendline), 0, (struct sockaddr *)&remote_addr, len);

    n = recvfrom(sockfd, recvline, 999, 0, (struct sockaddr *) &remote_addr, &len);
    //recvline[n] = 0;
    recvline[strlen(recvline)-1] = 0;
    printf("IP = %s, P = %d, msg = %s\n", inet_ntoa(remote_addr.sin_addr), ntohs(remote_addr.sin_port), recvline);

    while(1){
        
        n = recvfrom(sockfd, recvline, 999, 0, (struct sockaddr *) &remote_addr, &len);
        recvline[n] = 0;
        recvline[strlen(recvline)-1] = 0;
        printf("The server says = '%s'\n", recvline);
        // clear sendline
        memset(sendline, 0, BUFFER_SIZE);
        fgets(sendline, 1000, stdin);
        sendto(sockfd, sendline, strlen(sendline), 0, (struct sockaddr *)&remote_addr, len);

        n = recvfrom(sockfd, recvline, 999, 0, (struct sockaddr *) &remote_addr, &len);
        printf("The server says = '%s", recvline);
        if(strcmp(recvline, "endgame") == 0)
            break;
    }

    printf("End of comnunication\n");
    close(sockfd);

    return 0;
}