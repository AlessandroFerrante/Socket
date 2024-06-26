/**
 * @file serverUDP.c
 * @author Alessandro Ferrante
 * @brief this file represents the server in socket communication
 * @version 1.1
 * @date 2024-04-24
 * 
 * @copyright Copyright (c) 2024
 * 
**/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h> 

int main(int argc, char **argv){

    // socket descriptor
    int sockfd;
    // number of bytes received
    int nBytes;
    // local and remote address (server and client)
    struct sockaddr_in local_addr, remote_addr;
    socklen_t len =  sizeof(struct sockaddr_in);
    // Text received from the clients
    char msg[1000];
    // to store the address in text form
    char in_addr[INET_ADDRSTRLEN];

    if(argc != 2){
        printf("Error, please insert port number\n");
        return -1;
    }

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        printf("Error occured while opening, socket\n");
        return -1;
    }

    // fill memory with a constant byte, set all bytes to 0
    memset(&local_addr, 0, len);  
    // IPv4 address family
    local_addr.sin_family = AF_INET; 
    // convert the port number from host byte order to network byte order
    local_addr.sin_port = htons(atoi(argv[1])); 

    if(bind(sockfd, (struct sockaddr *) &local_addr, len) < 0){
        printf("Error! Insert a different number port\n");
        return -1;
    }

    for(;;) {
        nBytes = recvfrom(sockfd, msg, 999, 0, (struct sockaddr *)&remote_addr, &len);
        msg[nBytes] = 0;

        sendto(sockfd, msg, nBytes, 0, (struct sockaddr *) &remote_addr, len);
               
        // convert IPv4 and IPv6 addresses from binary to text form
        inet_ntop(AF_INET, &(remote_addr.sin_addr), in_addr, INET_ADDRSTRLEN);

        printf("IP = %s, P = %d, msg = %s\n", in_addr, ntohs(remote_addr.sin_port), msg);
        if(strcmp(msg, "end\n") == 0) {
            break;
        }
    }
    close(sockfd);
    return 0;
}