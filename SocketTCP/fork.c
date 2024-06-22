/**
 * @file fork.c
 * @author Alessandro Ferrante (dev@alessandroferrante.net)
 * @brief 
 * Esempio per far vedere la diferenza tra processo padre e processo figlio. |
 * Example to show the difference between parent process and child process.
 * 
 * Il fork duplica un processo esistente, il padre crea un clone di se stesso chiamato figlio, 
 * quest'ultimo eredita lo stato corrente, inclusi codice, dati e risorse. |
 * The fork duplicates an existing process, the parent creates a clone of itself called a child, 
 * the latter inherits the current state, including code, data and resources.
 * @version 0.1
 * @date 2024-04-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdio.h>
#include<string.h>
#include <unistd.h>

int main(int argc, char **argv){

    int n = 0, pid;
    for(; n<5; n++){
        printf("");
    }
    pid = fork();

    if(pid == 0){
        printf("\n========= Child \n n = %d \n", n);
        for(; n<5; n++){
            printf("n = %d\n", n);
        }
        return 0;
    }
    else{
        printf("\n========= Father \n n = %d \n", n);
        for(; n<5; n++){
            printf("n = %d\n", n*2);
        }
        return 0;
    }
}