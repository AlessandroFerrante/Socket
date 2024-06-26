/**
 * @file clientTCP.c
 * @author Alessandro Ferrante github@alessandroferrante.net
 * @brief 
 * @version 0.1
 * @date 2024-05-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char const *argv[]){
    
    int sockfd, n;
    struct sockaddr_in local_addr, remote_addr;
    char sendline[1000];
    char recvline[1000];  

    if (argc < 3){
        printf("Error! Insert destination IP and port\n");
        return -1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        printf("Errore nell'apertyra della socket\n");
        return -1;
    }

    memset(&remote_addr, 0,sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_addr.s_addr =  inet_addr(argv[1]);
    remote_addr.sin_port =  htons(atoi(argv[2]));
    
    connect(sockfd, (struct sockaddr *)&remote_addr, sizeof(remote_addr));

    while (fgets(sendline, 999, stdin) != NULL){
        send(sockfd, sendline, strlen(sendline), 0);
        n = recv(sockfd, recvline, 999, 0);
        recvline[n] = 0;
        printf("IP %s, P %d, msg %s\n",
            inet_ntoa(remote_addr.sin_addr),
            ntohs(remote_addr.sin_port), 
            recvline);
        if(strcmp(recvline, "fine\n") == 0){
            printf("Fine\n");
            break;
        }
    }
    
    return 0;
}