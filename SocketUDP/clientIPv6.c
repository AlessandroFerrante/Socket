/**
 * @file clientIPv6.c
 * @author Alessandro Ferrante github@alessandroferrante.net
 * @brief this file represents the client in socket communication with IPv6
 * @version 0.1
 * @date 2024-04-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char ** argv){
    int sockfd, n;
    struct sockaddr_in6 remote_addr;
    socklen_t len = sizeof(struct sock_addr_in6);
    char sendline[1000];
    char revline[1000];
    char ipv6_addr[INET_ADDRSTRLEN];

    if(argc != 3){
        printf("Error! Please insert Ip and port number\n");
        return -1;
    }

    sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if(sockfd < 0){
        printf("Error occured while socket opening\n");
        return -1;
    }

    memset(&remote_addr, 0, len);
    remote_addr.sin6_family = AF_INET6;
    inet_pton(AF_INET6, argv[1], &remote_addr.sin6_addr);
    remote_addr.sin6_port = htons(atoi(argv[2]));

    while(fgets(sendline, 1000, stdin) != NULL) {
        sendto(sockfd, sendline, strlen(sendline), 0, (struct sockaddr *)&remote_addr, len);
        n = recvfrom(sockfd, recline, 999, 0, (struct sockaddr *)&remote_addr, &len);
        recline[n] = 0;
        inet_ntop(AF_INET6, &(remote_addr.sin6_addr), ipv6_addr, INET6_ADDRSTRLEN);
        printf("IP = %s, P = %d, msg = %s\n", ipv6_addr, ntohs(remote_addr.sin6_port), recline);
        if(strcmp(recline, "end\n") == 0) break;
    }

    printf("End of communication\n");
    close(sockfd);

    return 0;
}