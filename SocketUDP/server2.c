/**
 * @file server2.c
 * @author Alessandro Ferrante github@alessandroferrante.net
 * @brief this file represents the server in socket communication
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

int main(int argc, char ** argv){

    int sockfd;
    int nBytes;
    struct sockaddr_in local_addr, remote_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    char buffer[1000];
    char in_addr[INET_ADDRSTRLEN];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    perror("sockfd: ");

    memset(&local_addr, 0, len);
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(atoi("8080"));
    inet_pton(AF_INET, "0.0.0.0", &local_addr.sin_addr);
    

    bind(sockfd, (struct sockaddr *) &local_addr, len);
    perror("bind: ");
    

    for(;;){
        nBytes = recvfrom(sockfd, buffer, 1000, 0, (struct sockaddr *)&remote_addr, &len);
        buffer[nBytes] =  0;
        
        inet_ntop(AF_INET, &remote_addr.sin_addr, in_addr, len);

        printf("Buffer: %s ", buffer);
        snprintf(buffer, 1000, "Test: %s", "testSTR" );
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *) &remote_addr, len);
    }

    close(sockfd);
    return 0;
}