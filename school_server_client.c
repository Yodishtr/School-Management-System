//
// Created by Yodishtr Vythilingum on 24/11/2025.
// Is the client for the school server.
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#define PORT 8000
#define SERVER_IP "127.0.0.1"

int main() {
    char buffer[1024];
    char message[1024];

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error in setting up client socket\n");
        return 1;
    }
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    // if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0){
    //     perror("inet_pton function failed");
    //     return 1;
    // }
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr) < 0)){
        perror("connect\n");
        return 1;
    }


    return 0;
}