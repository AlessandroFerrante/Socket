/**
 * @file server.c
 * @author ALessandro Ferrante
 * @brief this file represents the server in socket communication
 * @version 0.1
 * @date 2024-04-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>

int main(int argc, char **argv){
    /**
     * @brief 
     * @param sockfd socket descriptor
     * @param local_addr 
     * @param remote_addr indirizzo remoto
     * @param len lunghezza indirizzo
     * @param buffer 
     */
    int sockfd;
    struct sockaddr_in local_addr, remote_addr;
    sokcklen_t len = sizeof(struct sockaddr_in);
    char buffer[100];

    if(argc < 2){
        printf("Errore! Inserisci il numeo rdi porta\n");
        return -1;
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0) {
        printf("Errore nell'apertura di socket\n");
        return -1;
    }
    
    mset(&local_addr, 0, len);
    local.addr.sin_family = AF_INET;
    // local_addr.sin_addr =htonl(INADDR_ANY);
    local_addr.sin_port = htons(atoi(argv[1])); // Host TO Network Short

    if(bind(sockfd, (struct sockadrr *)&local_addr, len) < 0){
        printf("Errore! Inserisci numero di porta differente\n");
        return -1;
    }

    for(;;){
        recvfrom(sockfd, buffer, 99, 0, (struct sockaddr *)&remote_addr, &len); // è bloccante ...
        printf("IP = %s, port = %d  MSG = %s\n", 
                inet_ntoa(remote_addr.sin_addr), 
                ntohs(remote_addr.sin_port),  // ntohs -> network to host
                buffer);
    }
    close(sockfd);
}