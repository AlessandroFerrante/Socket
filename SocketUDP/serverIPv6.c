/**
 * @file serverIPv6.c
 * @author Alessandro Ferrante github@alessandroferrante.net
 * @brief this file represents the server in socket communication with IPv6
 * @version 0.1
 * @date 2024-04-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>

int main(int argc, char **argv){

    int sockfd, n;
    struct sockaddr_in6 local_addr, remote_addr;
    scoklel_t len =  sizeof(struct sock_addr_in6);
    char ipv6_addr[46];
    char msg[1000];
    
    if(argc != 2){
        printf("Error, please insert port number\n");
        retunr -1;
    }

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        printf("Error occured while opening, socket\n");
        return -1;
    }

    memset(&remote_addr, 0, len);
    local_addr.sin6_family = AF_INET6;
    local_addr.sin6_port = htons(atoi(argv[1]));

    if(bind(sockfd, (struct sockaddr *)&local_addr, len) < 0) {
        printf("Error while binding socket\n");
        return -1;
    } 
    
    for(;;) {
        n = recvfrom(sockfd, msg, 999, 0, (struct sockaddr *)&remote_addr, &len);
        msg[n] = 0;

        sendto(sockfd, msg, n, 0, (struct sockaddr *)&remote_addr, len);

        inet_ntop(AF_INET6, &(remote_addr.sin6_addr), ipv6_addr, INET6_ADDRSTRLEN);

        printf("IP = %s, P = %d, msg = %s\n", ipv6_addr, ntohs(remote_addr.sin6_port), msg);
        if(strcmp(msg, "end\n") == 0) {
            break;
        }
    }
    close(sockfd);
    return 0;
}