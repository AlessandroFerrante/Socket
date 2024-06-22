/**
 * @file client.c
 * @author ALessandro Ferrante
 * @brief this file represents the client in socket communication
 * @version 0.1
 * @date 2024-04-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int main (int argc, char ** argv) {
    /**
     * @brief 
     * @param sockfd socket descripton, contiene un numero intero univo che identifica la socket
     * @param dest_addr destination address
     * @param buffer text to send to server
     */
    int sockfd;
    struct sockaddr_in dest_addr;
    char buffer[100];

    if(argc < 3) {
        printf("Error! Insert destination IP and port\n")
        return -1;
    }

    /**
     * @brief 
     * @param sockfd is used to store the unique value of the sccket
     * @param socket system call to create a communication endpoint
     * @param PF_INET specifies the address family, indicates that it uses Internet Protocol (Protocol Family: Internet)
     * @param SOCK_DGRAM specifies the socket type Datagram Socket
     * @param 0 indicates the protocol to use, in this case choose the operating system
     */
    //if((sockfd = sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        printf("Error opening socket\n");
        return -1;
    }
    
    /**
     * @brief 
     * 
     */
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    /**
     * @brief 
     * transforms from string to network format
     * @param &(dest_addr.sin_addr) to get the address
     */
    inet_pton(AF_INET, argv[1], &(dest_addr.sin_addr));
    // to set the port, converting the number passed into the argument
    dest_addr.sin_port = htons(atoi(argv[2]));

    // sending and/or receiving phase
    for(int = 0; i<10; i++){
        sprintf(buffer, "%d", i);
        sendto(sockfd, buffer, sizeof(buffer)+1, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    }
    
    close(sockfd);
    return 0;
}