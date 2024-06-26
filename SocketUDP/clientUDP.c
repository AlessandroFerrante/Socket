/**
 * @file sendUDP.c
 * @author Alessandro Ferrante
 * @brief this file represents the client in socket communication
 * @version 1.1
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

int main(int argc, char **argv){
    int sockfd, n;
    struct sockaddr_in remote_addr;
    char sendline[1000];
    char recvline[1000];
    socklen_t len =  sizeof(struct sockaddr_in);

    if(argc!=3) {
        printf("Errore Inserisci IP e port\n");
        return -1;
    }

    sockfd =  socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0) {
        printf("Errore nell'apertura di socket\n");
        return -1;
    }

    memset(&remote_addr, 0, len);
    remote_addr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &(remote_addr.sin_addr));
    remote_addr.sin_port = ntohs(atoi(argv[2]));

    while(fgets(sendline, 1000, stdin) != NULL){
        sendto(sockfd, sendline, strlen(sendline), 0, 
                (struct sockaddr *)&remote_addr, len);

        n = recvfrom(sockfd, recvline, 999, 0, (struct sockaddr*)&remote_addr, &len);
        recvline[n] = 0;

        printf("IP = %s, P = %d, msg = %s\n", 
                    inet_ntoa(remote_addr.sin_addr),
                    ntohs(remote_addr.sin_port),
                    recvline);
        if(strcmp(recvline, "fine\n") == 0)
            break;

        printf("Fine comunicazione\n");
        close(sockfd);
    }

    return 0;
}