#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>

#define SERVERPORT 5555
#define MAXBYTE 1024

int main(int argc, char **argv) {
    char buffer[MAXBYTE];
    char message[MAXBYTE];
    int sock;
    int byteReads, len;
    struct sockaddr_in servAddr;

    // clearing servAddr
    memset(&servAddr,0, sizeof(servAddr));
    servAddr.sin_addr.s_addr = inet_addr("0.0.0.0");
    servAddr.sin_port = htons(SERVERPORT);
    servAddr.sin_family = AF_INET;

    // creating datagram socket
    if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("sock error\n");
        exit(0);
    }

    int turn = 0;
    int start = 0;
    while(1) {
        if(turn == 0) { // write state: send text to server
            memset(&message, 0, strlen(message)); // clearing the buffer
            
            printf("[*] ƪ(ړײ)‎ƪ client: ");
            fgets(message, MAXBYTE, stdin);
            if(strncmp(message, "over", 4) != 0) {
                message[strlen(message)] = '\0';
                sendto(sock, message, strlen(message), 0, 
                 (struct sockaddr*)&servAddr, sizeof(servAddr));
            }else {
                message[strlen(message)] = '\0';
                sendto(sock, message, strlen(message), 0, 
                 (struct sockaddr*)&servAddr, sizeof(servAddr));
                 turn = 1;
            }
            if(start != 1) {
                memset(&buffer, 0, strlen(buffer)); // clearring the buffer
                recvfrom(sock, buffer, MAXBYTE, 0, 
                    (struct sockaddr*)&servAddr, &len);
                if(strncmp(buffer, "start", 5) == 0) {
                    start = 1;
                } else {
                    printf("[*] ʕ•ᴥ•ʔ server: %s", buffer);
                }
            }
        }

        
        if(turn == 1) { // read state: receives server message
            memset(&buffer, 0, strlen(buffer));
            byteReads = recvfrom(sock, buffer, MAXBYTE, 0, 
                (struct sockaddr*)&servAddr, &len);
            if(strncmp(buffer, "over", 4) != 0) {
                printf("[*] ʕ•ᴥ•ʔ server: %s", buffer);
            }else {
                printf("[*] ʕ•ᴥ•ʔ server: %s", buffer);
                turn = 0;
            }
        }
        


    }



}