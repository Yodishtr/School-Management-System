//
// Created by Yodishtr Vythilingum on 16/11/2025.
//
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8000

int AddStudent (int student_number, char *student_name, int student_age, char *program);

int main() {
    // setting up the server functionality
    int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_socket == -1) {
        perror("socket");
        return 1;
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(server_addr.sin_zero), 0, 8);

    int binding = bind(listen_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (binding == -1) {
        perror("bind");
        close(listen_socket);
        exit(1);
    }

    if (listen(listen_socket, 5) < 0) {
        perror("listen");
        exit(1);
    }

    struct sockaddr_in client_addr;
    unsigned int client_sock_length = sizeof(client_addr);
    client_addr.sin_family = AF_INET;
    int connection = accept(listen_socket, (struct sockaddr *)&client_addr, &client_sock_length);
    if (connection == -1) {
        perror("accept");
        exit(1);
    }

    return 0;
}

// Functions:

int AddStudent (int student_number, char *student_name, int student_age, char *program) {
    // student number is 16 numbered, student name is 50 chars max, student age is 3 number max
    // program is 3 letter
    char stdnt_num[17];
    char stdnt_age[4];
    sprintf(stdnt_num, "%d", student_number);
    sprintf(stdnt_age, "%d", student_age);

    FILE *file_ptr;
    file_ptr = fopen("student_record.txt", "a");
    if (file_ptr == NULL) {
        printf("Error when opening file\n");
        return 1;
    }
    // make an array containing the string versions of the arguments then add that to the file.


    return 0;
}