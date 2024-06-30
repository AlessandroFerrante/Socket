/**
 * @file client2.c
 * @author Alessandro Ferrante github@alessandroferrante.net
 * @brief this file represents the client in socket communication
 * @version 2.1
 * @date 2024-06-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main (int argc, char ** argv) {
    int sockfd;
    int nBytes;
    struct sockaddr_in dest_addr;
    char buffer[100];

    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    perror("sockfd: ");

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(atoi("8080"));
        
    inet_pton(AF_INET, "0.0.0.0", &(remote_addr.sin_addr));
    
    while(1){
        
        nBytes = recvfrom(sockfd, recvline, 999, 0, (struct sockaddr *) &remote_addr, &len);
        recvline[nBytes] = 0;
        recvline[strlen(recvline)-1] = 0;
        printf("The server says = '%s'\n", recvline);
        // clear buffer
        memset(buffer, 0, BUFFER_SIZE);
        fgets(buffer, 1000, stdin);
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&remote_addr, len);

        nBytes = recvfrom(sockfd, recvline, 999, 0, (struct sockaddr *) &remote_addr, &len);
        printf("The server says = '%s", recvline);
        if(strcmp(recvline, "endgame") == 0)
            break;
    }
    
    close(sockfd);
    return 0;
}