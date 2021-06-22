#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 5555
#define MAXBYTE 1024

int main(int argc, char **argv) {
    int sock;
    char buffer[MAXBYTE];
    char message[MAXBYTE] = "Welcome to chat server, enter start to continue...\n";
    struct sockaddr_in servAddr, clientAddr;
    
    printf("[*] Simple UDP Chat Server...\n");
    // opening socket
    if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("[*] failed to create socket");
        exit(EXIT_FAILURE);
    }
    // zero out the struct
    memset(&servAddr, 0, sizeof(servAddr));
    memset(&clientAddr, 0, sizeof(clientAddr));

    // setting server information
    servAddr.sin_family = AF_INET;  // family IPv4
    servAddr.sin_addr.s_addr = INADDR_ANY;
    servAddr.sin_port = htons(PORT);

    // binding socket with server address
    if(bind(sock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0) {
        perror("[*] failed to bind");
        exit(EXIT_FAILURE);
    }
    printf("[*] Server has started with ip: %s and port: %d\n",
            inet_ntoa(servAddr.sin_addr), PORT);
    int len, bytesReceived;
    int start = 0;
    int turn = 0; // if turn is 0 client has to send message, if turn is 1 server has to send message
    while(1) {
        len = sizeof(clientAddr);
        bytesReceived = recvfrom(sock, (char*)buffer, MAXBYTE,0,
                         (struct sockaddr*) &clientAddr, &len);
        buffer[bytesReceived] = '\0';  // adding null terminator
        
        if(strlen(buffer) > 0) {
            
            if(strncmp(buffer, "start",5) == 0) {
                if(start != 1) {
                    printf("[*] Connection established with client ip: %s and port: %d\n",
                        inet_ntoa(clientAddr.sin_addr), htons(clientAddr.sin_port));
                    printf("[*] ƪ(ړײ)‎ƪ client: %s",buffer);
                    start = 1;
                    memset(&message, 0, strlen(message));
                    strncpy(message, "start",5);
                    message[strlen(message)] = '\0';
                    sendto(sock, message, strlen(message), 0, 
                         (struct sockaddr*)&clientAddr, sizeof(clientAddr));
                }
            }else if(strncmp(buffer, "start", 5) != 0 && start == 0) {
                message[strlen(message)] = '\0';
                sendto(sock, message, strlen(message), 0, 
                    (struct sockaddr*)&clientAddr, sizeof(clientAddr));
            }
        }

        while(start == 1) { // one the chat is started this loops begins
            memset(&buffer, 0, strlen(buffer)); // clearing buffer before receiving
            if(turn == 0) {
                bytesReceived = recvfrom(sock, (char*)buffer, MAXBYTE,0,
                (struct sockaddr*) &clientAddr, &len);
            }
            if(turn == 0 && (strncmp(buffer, "over",4) != 0)) {
                buffer[bytesReceived] = '\0';  // adding null terminator
                printf("[*] ƪ(ړײ)‎ƪ client: %s",buffer);
            } else if(turn == 0 && (strncmp(buffer, "over",4) == 0)) {
                buffer[bytesReceived] = '\0';  // adding null terminator
                printf("[*] ƪ(ړײ)‎ƪ client: %s",buffer);
                turn  = 1;
            }

            if(turn == 1) {
                memset(&message, 0, strlen(message)); // clearing message before sending
                printf("[*] ʕ•ᴥ•ʔ server: ");
                // scanf("%s",&message);
                // int br = getline(&message, &maxlen , stdin);
                fgets(message, MAXBYTE, stdin);
                if(strlen(message) > 0) {
                    if(strncmp(message, "over",4) !=0) {
                        message[strlen(message)] = '\0';
                        sendto(sock, message, strlen(message), 0, 
                         (struct sockaddr*)&clientAddr, sizeof(clientAddr));
                    }else {
                        message[strlen(message)] = '\0';
                        sendto(sock, message, strlen(message), 0, 
                         (struct sockaddr*)&clientAddr, sizeof(clientAddr));
                         turn = 0;              
                    }
                }

            }
        }

    }







    return 0;
}