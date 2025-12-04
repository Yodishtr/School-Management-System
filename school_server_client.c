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

ssize_t read_line(int fd, char *buf, size_t max_len) {
    ssize_t total = 0;
    while (total < (ssize_t)(max_len - 1)) {
        char c;
        ssize_t n = read(fd, &c, 1);
        if (n == -1) {
            return -1;
        } else if (n == 0) {
            break;
        }
        if (c == '\n') {
            break;
        }
        buf[total++] = c;
    }
    buf[total] = '\0';
    return total;
}

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
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        perror("connect\n");
        return 1;
    }
    while (1) {

        printf("Server Function Codes. Select one:\n");
        printf("1. To add student: AS\n");
        printf("2. To find student by id: FSI\n");
        printf("3. To find student by name: FSN\n");
        printf("4. To get a list of the students in a program: SIP\n");
        printf("5. To get the total number of students: TS\n");
        printf("6. To delete a student by their id: DSBI\n");
        printf("7. To delete a student by their name: DSBN\n");
        printf("8. To update a student's id: USI\n");
        printf("9. To update a student's name: USN\n");
        printf("10. To update a student's program: USP\n");
        printf("Type either Q, q, or quit to exit.\n");

        char func_code[5];
        printf("Function code chosen: ");
        scanf("%s", func_code);
        if (strcmp(func_code, "Q") == 0 || strcmp(func_code, "q") == 0 || strcmp(func_code, "quit") == 0) {
            break;
        }
        write();
        printf("Server will send you prompts to answer.\n");
        printf("If you get a success, error or result message, type b to go back to menu.\n");
        printf("Add a newline (ie press enter) after answering the prompts");
        while (1) {
            if (read_line()){}
            char response[1024];

        }


    }


    return 0;
}