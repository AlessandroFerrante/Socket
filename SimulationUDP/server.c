/**
 * @file server.c
 * @author Alessandro Ferrante github@alessandroferrante.net
 * @brief 
 * EN| This code implements a server that uses UDP sockets to allow clients to play the hangman game.
 * Clients connect to the server, receive a game state (the word with guessed letters and attempts left),
 * send the letters to guess, and the server responds by updating the game state until a client wins or loses.
 * Only 2 clients can play per game and the game will only start when the server has registered the two players.
 * The server randomly chooses a string to use for the game.
 * ------
 * IT| Questo codice implementa un server che utilizza socket UDP per permettere ai client di giocare al gioco dell'impiccato.
 * I client si connettono al server, ricevono uno stato del gioco (la parola con lettere indovinate e tentativi rimasti),
 * inviano le lettere da indovinare, e il server risponde aggiornando lo stato del gioco fino a quando un client vince o perde.
 * Possono giocare solo 2 client a partita e il gioco inizierà solo quando il server avrà registrato i due giocatori.  
 * Il server sceglie randomicamente una stringa da utilizzare per il gioco.
 * @version 0.1
 * @date 2024-04-24
 * 
 * @copyright Copyright (c) 2024
 * 
**/

#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include <stdbool.h>
#include <time.h>

#define MAX_CLIENT 2
#define MAX_NAMESIZE 15
#define BUFFER_SIZE 1000

typedef struct {
    char name[MAX_NAMESIZE];
    in_addr_t ip;
    int port;
    int lives;
} Client;

int main(int argc, char **argv){
       
    // socket descriptor
    int sockfd; 
    // number of bytes received
    int nBytes = 0;  
    // local and remote address (server and client)
    struct sockaddr_in local_addr, remote_addr;
    socklen_t remoteaddr_len =  sizeof(struct sockaddr_in);
    // Text received from the clients
    char buffer[BUFFER_SIZE];
    char wrds[][10] = {"Ciao", "Hello", "Mondo", "World"};

    Client players[MAX_CLIENT];
    int turn = -1;

    /*
    if(argc != 2){
        printf("Error, please insert port number\n");
        return -1;
    }
    */

    srand(time(NULL));
    char *word_to_guess = wrds[rand() % (sizeof(wrds) / sizeof(char *))];
    char *word_structure = strdup(word_to_guess); 
    for (size_t i = 0; i < strlen(word_structure); i++) word_structure[i]= '_';

    printf("Word to guess: %s \nWord structure: %s\n", word_to_guess, word_structure);

    // creates  an endpoint for communication and returns a file descriptor that refers to that endpoint
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    perror("Socket opening "); // if socket caused an error


    // fill memory with a constant byte, set all bytes to 0
    memset(&local_addr, 0, remoteaddr_len);  
    // IPv4 address family
    local_addr.sin_family = AF_INET; 
    // convert the port number from host byte order to network byte order
    local_addr.sin_port = htons(atoi("8080")); 
    // convert addresses from text to binary form
    // set the server address
    inet_pton(AF_INET, "0.0.0.0", &local_addr.sin_addr);

    // try to bind the socket connection to the specified local address and port
    bind(sockfd, (struct sockaddr *)&local_addr, remoteaddr_len);
    perror("Bind "); // if bind caused an error

    // waiting for players
    for (size_t i = 0; i < MAX_CLIENT; i++) {
        printf("%s %lu\n", "Waiting for the player", i);

        // clear structure
        memset(buffer, 0, BUFFER_SIZE);
        memset(&players[i], 0, sizeof(Client));
        // stores data sent by the player
        nBytes = recvfrom(sockfd, buffer, MAX_NAMESIZE, 0, (struct sockaddr*) &remote_addr, &remoteaddr_len ); 
        buffer[strlen(buffer)-1] = 0;
        strcpy(players[i].name, buffer);
        players[i].ip = remote_addr.sin_addr.s_addr;
        players[i].port = remote_addr.sin_port;
        players[i].lives = 3;
    
        printf("Player registration: %s with IP %s and port number %d, completed!\n", players[i].name, inet_ntoa(remote_addr.sin_addr), players[i].port);  

        memset(buffer, 0, BUFFER_SIZE);
        snprintf(buffer, BUFFER_SIZE, "Player registration: %s with ip %d and port number %d, completed\n", players[i].name, players[i].ip, players[i].port);
        sendto(sockfd, buffer, strlen(buffer) + 1, 0, (struct sockaddr *) &remote_addr, remoteaddr_len);

    }
    
    // logic and rules of the game
    int current_player = 0;
    while(players[0].lives > 0 || players[1].lives > 0) {

        if (players[current_player].lives == 0){
            current_player = 1- current_player;
            continue;
        }
        
        remote_addr.sin_addr.s_addr = players[current_player].ip;
        remote_addr.sin_port = players[current_player].port;
        
        
        memset(buffer, 0, BUFFER_SIZE);
        snprintf(buffer, sizeof(buffer), "Structure of the word to guess %s, Lives %s: %d - %d : %s\n", word_structure, players[0].name, players[0].lives,  players[1].lives, players[1].name);
        sendto(sockfd, buffer, strlen(buffer) + 1, 0, (struct sockaddr *) &remote_addr, remoteaddr_len);
        
        memset(buffer, 0, BUFFER_SIZE);
        recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&remote_addr, &remoteaddr_len);
        buffer[strlen(buffer)-1] = 0;

        int lives_lost = 1;
        if (strlen(buffer) == 1) {

            char letter = buffer[0];
            for (size_t i = 0; i < strlen(word_to_guess); i++) {
                if(word_to_guess[i] == letter){
                    lives_lost = 0;
                    word_structure[i] = letter;
                }
            }   
        }
        else if(strcmp(word_to_guess, buffer) == 0) {
            lives_lost = 0;
            strcpy(word_structure, word_to_guess);
        }
            
        if (lives_lost) players[current_player].lives--;

        memset(buffer, 0, BUFFER_SIZE);
        snprintf(buffer, sizeof(buffer), "Structure of the word to guess %s, Lives %s: %d - %d : %s\n", word_structure, players[0].name, players[0].lives,  players[1].lives, players[1].name);
        sendto(sockfd, buffer, strlen(buffer) + 1, 0, (struct sockaddr *) &remote_addr, remoteaddr_len);
        
        if(strchr(word_structure, '_') == NULL){
            printf("End game");
            
            memset(buffer, 0, BUFFER_SIZE);
            snprintf(buffer, sizeof(buffer), "You won! The word was %s | Lives %s: %d - %d : %s\n", word_structure, players[0].name, players[0].lives,  players[1].lives, players[1].name);
            sendto(sockfd, buffer, strlen(buffer) + 1, 0, (struct sockaddr*)&remote_addr, remoteaddr_len);
            sendto(sockfd, "endgame", 8, 0, (struct sockaddr *) &remote_addr, remoteaddr_len);
            
            printf("%s won!",  players[current_player].name);
            perror("sendto win");
            
            
            remote_addr.sin_addr.s_addr = players[1 - current_player].ip;
            remote_addr.sin_port = htons(players[1 - current_player].port);
            
            memset(buffer, 0, BUFFER_SIZE);
            snprintf(buffer, sizeof(buffer), "Game over! | The word was %s | Lives %s: %d - %d : %s\n", word_structure, players[0].name, players[0].lives,  players[1].lives, players[1].name);
            sendto(sockfd, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)&remote_addr, remoteaddr_len);
            sendto(sockfd, "endgame", 8, 0, (struct sockaddr *) &remote_addr, remoteaddr_len);
            
            printf("%s lost!",  players[1 - current_player].name);
            perror("sendto lose");

        }

        if (players[current_player].lives == 0 || strchr(word_structure, '_') == NULL){
            memset(buffer, 0, BUFFER_SIZE);
            snprintf(buffer, sizeof(buffer), "Game over! | The word was %s | Lives %s: %d - %d : %s\n", word_structure, players[0].name, players[0].lives,  players[1].lives, players[1].name);
            sendto(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &remote_addr, remoteaddr_len);
            perror("sendto lose");
            break;
        }

        current_player = 1 - current_player;
    }

    printf("End of game\n");

    free(word_structure);
    close(sockfd);
    return 0;
}
