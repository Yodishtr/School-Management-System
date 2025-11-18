//
// Created by Yodishtr Vythilingum on 16/11/2025.
//
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8000

// Structs needed
typedef struct student {
    int std_number;
    int std_age;
    char std_name[50];
    char std_program[5];
} Student;

// Function prototypes
int AddStudent (int student_number, char *student_name, int student_age, char *program);
int FindStudentId(int student_number, Student *alloc_mem);
int FindStudentName(char *student_name, Student *mem_alloc);




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

    // once connected: implement the connection loop to converse with client

    return 0;
}

// Functions:

int AddStudent (int student_number, char *student_name, int student_age, char *program) {
    // returns 0 on error and 1 on success.

    Student new_student;
    new_student.std_number = student_number;
    new_student.std_age = student_age;
    strncpy(new_student.std_name, student_name, 50);
    strncpy(new_student.std_program, program, 5);
    if (strlen(student_name) >= 50) {
        new_student.std_name[49] = '\0';
    }
    if (strlen(program) >= 5) {
        new_student.std_program[4] = '\0';
    }

    FILE *file_ptr;
    file_ptr = fopen("student_record.txt", "ab");
    if (file_ptr == NULL) {
        printf("Error when opening file\n");
        return 1;
    }
    size_t student_written = fwrite(&new_student, sizeof(Student), 1, file_ptr);
    if (student_written != 1) {
        perror("Error when writing to the file");
        fclose(file_ptr);
        return 1;
    }
    fclose(file_ptr);
    return 0;
}

int FindStudentId(int student_number, Student *alloc_mem) {
    // returns 0 on success and 1 on failure
    // stores the retrieved student data in alloc_mem which is heap memory that was pre-emptively
    // allocated before FindStudent was called
    FILE *file_ptr = fopen("student_record.txt", "rb");
    if (file_ptr == NULL) {
        perror("Error in opening the file\n");
        return 1;
    }

    Student temp;
    while (fread(&temp, sizeof(Student), 1, file_ptr) == 1) {
        if (temp.std_number == student_number) {
            *alloc_mem = temp;
            return 0;
        }
    }
    fclose(file_ptr);
    return 1;
}

int FindStudentName(char *student_name, Student *mem_alloc) {
    // returns 0 on success and 1 on failure
    // mem_alloc is pointer to address allocated on heap to store correct student struct
    FILE *file_ptr = fopen("student_record.txt", "rb");
    if (file_ptr == NULL) {
        perror("Error opening the file\n");
        return 1;
    }

}
