/**
 * @file serverTCP.c
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
    
    int sockfd, newsockfd, n;
    struct sockaddr_in local_addr, remote_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    char msg[1000];

    if(argc < 2) {
        printf("Error! Insert destination IP and port\n");
        return -1;
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("Error opening socket\n");
        return -1;
    }    

    memset(&local_addr, 0, len);
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr =  htonl(INADDR_ANY);
    local_addr.sin_port = htons(atoi(argv[1]));
    
    if(bind(sockfd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0){
        printf("Errore! Inserisci numero di porta differente\n");
        return -1;
    }

    listen(sockfd, 5);

    for (;;){
        newsockfd = accept(sockfd, (struct sockaddr *)&remote_addr, &len);
        // sokcfd newsockfd 
        if (fork() == 0){
            // the son will do this
            close(sockfd);

            for(;;){
                n = recv(newsockfd, msg, 999, 0);
                msg[n] = 0;
                printf("IP %s, P %d, msg %s\n",
                    inet_ntoa(remote_addr.sin_addr),
                    ntohs(remote_addr.sin_port), 
                    msg);
                
                send(newsockfd, msg, n, 0);

                if(strcmp(msg, "fine\n") == 0){
                    printf("Connection end\n");
                    return -1;
                }
            }

            close(newsockfd);
            return 0;

        }
        else{
            // the father will do this
            
            close(newsockfd);
        }
        
    }
    

    return 0;
}