//
// Created by Yodishtr Vythilingum on 16/11/2025.
//
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#define PORT 8000

// Structs needed
typedef struct student {
    int std_number;
    int std_age;
    char std_name[50];
    char std_program[5];
    int deleted; // 0 for not deleted, 1 for deleted
} Student;

// Server Function prototypes:
// Code that the client will send to server to determine which function to use:
// for e.g client will send: A, S, 1, J, O, H, N, 1, 5, C, S, C
// first 2 to 4 elements will be server function client wants to use, next is student number, etc...
// important to not have any space between the characters
int AddStudent (int student_number, char *student_name, int student_age, char *program);
int FindStudentId(int student_number, Student *alloc_mem);
int FindStudentName(char *student_name, Student *mem_alloc);
Student *StudentsInProgram(char *program, int *student_count);
int TotalStudents();
int DeleteStudentById(int student_id);
int DeleteStudentByName(char *student_name);
int UpdateStudentId(int student_id, int new_student_id);
int UpdateStudentName(char *student_name, char *new_name);
int UpdateStudentProgram(int student_id, char *new_program);

// Helper Function Prototypes
int GetServerFunction(char *client_response, char *func_code);
int GetStudentNumber(char *client_response, int start_index, char *student_id);
char *GetStudentName(char *client_response, int start_index, int *next_func_start);
int GetStudentAge(char *client_response, int start_index, int *student_age);
char *GetStudentProgram(char *client_response, int start_index); // will always assume student age is 2 elements


int GetServerFunction(char *client_response, char *func_code) {
    // returns the ending index for the function code
    // returns -1 if the function code is not valid
    char temp_buff[5];
    int i;
    for (i = 0; i < 5; i++) {
        if (!isalpha(client_response[i])) {
            break;
        }
        temp_buff[i] = client_response[i];
    }
    temp_buff[4] = '\0';
    int buff_len = strlen(temp_buff);
    if (buff_len == 0 || buff_len == 1 || buff_len > 4) {
        return -1;
    }
    if (strcmp(temp_buff, "AS") == 0) {
        // AddStudent Function code
        strncpy(func_code, temp_buff, 2);
        func_code[2] = '\0';

    } else if (strcmp(temp_buff, "FSI") == 0) {
        // FindStudentId function code
        strncpy(func_code, temp_buff, 3);
        func_code[3] = '\0';
    } else if (strcmp(temp_buff, "FSN") == 0) {
        // FindStudentName function code
        strncpy(func_code, temp_buff, 3);
        func_code[3] = '\0';
    } else if (strcmp(temp_buff, "SIP") == 0) {
        // StudentsInProgram function code
        strncpy(func_code, temp_buff, 3);
        func_code[3] = '\0';
    } else if (strcmp(temp_buff, "TS") == 0) {
        // TotalStudents function code
        strncpy(func_code, temp_buff, 2);
        func_code[2] = '\0';
    } else if (strcmp(temp_buff, "DSBI") == 0) {
        // DeleteStudentById function code
        strncpy(func_code, temp_buff, 4);
        func_code[4] = '\0';
    } else if (strcmp(temp_buff, "DSBN") == 0) {
        // DeleteStudentByName function code
        strncpy(func_code, temp_buff, 4);
        func_code[4] = '\0';
    } else if (strcmp(temp_buff, "USI") == 0) {
        // UpdateStudentId function code
        strncpy(func_code, temp_buff, 3);
        func_code[3] = '\0';
    } else if (strcmp(temp_buff, "USN") == 0) {
        //UpdateStudentName function code
        strncpy(func_code, temp_buff, 3);
        func_code[3] = '\0';
    } else if (strcmp(temp_buff, "USP") == 0) {
        // UpdateStudentProgram func code
        strncpy(func_code, temp_buff, 3);
        func_code[3] = '\0';
    } else {
        printf("Incorrect Function code provided\n");
        return -1;
    }

    return i;

}

int GetStudentNumber(char *client_response, int start_index, char *student_id) {
    // returns the ending index + 1 of the student number
    // returns -1 otherwise
    char temp_buffer[10];
    int i;
    int j = 0;
    int n = start_index + 10 + 1;
    for (i = start_index; i < n; i++) {
        if (client_response[i] >= 0 && client_response[i] <= 9) {
            temp_buffer[j] = client_response[i];
            j++;
        } else {
            break;
        }
    }
    if (j == 10) {
        temp_buffer[j - 1] = '\0';
    } else if (j > 0 && j < 10) {
        temp_buffer[j] = '\0';
    } else {
        return -1;
    }
    int curr_len = strlen(temp_buffer);
    strncpy(student_id, temp_buffer, curr_len);
    student_id[curr_len + 1] = '\0';
    return i;
}

char *GetStudentName(char *client_response, int start_index, int *next_func_start) {
    // returns the name of the student
    // returns NULL on failure
    char *student_name = malloc(sizeof(char) * 50);
    int i;
    int j = 0;
    for (i = start_index; i < start_index + 50 + 1; i++) {
        if (!isalpha(client_response[i])) {
            break;
        }
        student_name[j] = client_response[i];
        j++;
    }
    student_name[j] = '\0';
    if (strlen(student_name) == 0) {
        return NULL;
    }
    *next_func_start = i;
    return student_name;
}

int GetStudentAge(char *client_response, int start_index, int *student_age) {
    // returns the starting index for the program
    // returns -1 for any failures or errors
    int i;
    char std_age[3];
    for (){}
}

char *GetStudentProgram(char *client_response, int start_index) {
    // returns pointer to the string of the client response for the program
    // returns NULL
    char *student_program = malloc(sizeof(char) * 5);
    int i = start_index;
    int j = 0;
    while (client_response[i] != '\0') {
        student_program[j] = client_response[i];
        i++;
        j++;
    }
    student_program[j] = '\0';
    if (strlen(student_program) == 0) {
        return NULL;
    }
    return student_program;
}


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
    socklen_t client_sock_length = sizeof(client_addr);
    client_addr.sin_family = AF_INET;
    int connection = accept(listen_socket, (struct sockaddr *)&client_addr, &client_sock_length);
    if (connection == -1) {
        perror("accept");
        exit(1);
    }

    // once connected: implement the connection loop to converse with client
    while (1) {
        char buffer[1024];
        char response[1024];
        ssize_t bytes_received = read(connection, buffer, sizeof(buffer));
        if (bytes_received == -1) {
            perror("Error in client message\n");
        } else if (bytes_received == 0) {
            perror("Server disconnected. Server will close. Restart the server to continue\n");
            break;
        }
        buffer[bytes_received + 1] = '\0';


    }

    close(listen_socket);
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
    new_student.deleted = 0;
    if (strlen(student_name) >= 50) {
        new_student.std_name[49] = '\0';
    }
    if (strlen(program) >= 5) {
        new_student.std_program[4] = '\0';
    }

    FILE *file_ptr;
    file_ptr = fopen("student_record.dat", "ab");
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
    // returns 0 on success and 1 on failure and 2 if no student
    // stores the retrieved student data in alloc_mem which is heap memory that was pre-emptively
    // allocated before FindStudent was called
    FILE *file_ptr = fopen("student_record.dat", "rb");
    if (file_ptr == NULL) {
        perror("Error in opening the file\n");
        return 1;
    }

    Student temp;
    while (fread(&temp, sizeof(Student), 1, file_ptr) == 1) {
        if ((temp.std_number == student_number) && (temp.deleted == 0)) {
            *alloc_mem = temp;
            fclose(file_ptr);
            return 0;
        }
    }
    fclose(file_ptr);
    return 2;
}

int FindStudentName(char *student_name, Student *mem_alloc) {
    // returns 0 on success and 1 on failure and 2 on no student found
    // mem_alloc is pointer to address allocated on heap to store correct student struct
    FILE *file_ptr = fopen("student_record.dat", "rb");
    if (file_ptr == NULL) {
        perror("Error opening the file\n");
        return 1;
    }
    Student temp;
    while (fread(&temp, sizeof(Student), 1, file_ptr) == 1) {
        if (strcmp(temp.std_name, student_name) == 0 && temp.deleted == 0) {
            *mem_alloc = temp;
            fclose(file_ptr);
            return 0;
        }
    }
    fclose(file_ptr);
    return 2;
}

Student *StudentsInProgram(char *program, int *student_count) {
    // returns a pointer to memory on heap where the array of students in program
    // returns NULL if no students found. sets the count to the number of students in
    // the program. student count should have been previously initialized & its addressed passed
    // to the function

    int set_count = 0;
    FILE *file_ptr = fopen("student_record.dat", "rb");
    if (file_ptr == NULL) {
        perror("Error when opening the file\n");
        return NULL;
    }
    Student temp;
    while (fread(&temp, sizeof(Student), 1, file_ptr) == 1) {
        if (strcmp(temp.std_program, program) == 0 && temp.deleted == 0) {
            set_count++;
        }
    }
    if (set_count == 0) {
        fclose(file_ptr);
        return NULL;
    } else {
        rewind(file_ptr);
        Student *heap_ptr = (Student *)malloc(set_count * sizeof(Student));
        if (heap_ptr == NULL) {
            perror("Error in dynamic/heap memory allocation");
            fclose(file_ptr);
            return NULL;
        }
        int i;
        for (i = 0; i < set_count; i++) {
            Student temp2;
            if (fread(&temp2, sizeof(Student), 1, file_ptr) == 1) {
                if (strcmp(temp2.std_program, program) == 0 && temp2.deleted == 0) {
                    heap_ptr[i] = temp2;
                }
            }
        }
        *student_count = set_count;
        fclose(file_ptr);
        return heap_ptr;
    }
}

int TotalStudents() {
    // returns the total number of students
    FILE *file_ptr = fopen("student_record.dat", "rb");
    if (file_ptr == NULL) {
        perror("Error in opening file\n");
        return -1;
    }
    int total = 0;
    Student some_temp;
    while (fread(&some_temp, sizeof(Student), 1, file_ptr) == 1) {
        if (some_temp.deleted == 0) {
            total++;
        }
    }
    fclose(file_ptr);
    return total;
}

int DeleteStudentById(int student_id) {
    // returns 0 if deletion successful and returns 1 otherwise.
    // change the delete flag by overwriting it at that specific position
    // dont forget to open file in a binary read and write mode if thats possible.
    FILE *file_ptr = fopen("student_record.dat", "rb+");
    if (file_ptr == NULL) {
        perror("error opening the file\n");
        return 1;
    }
    Student temp;
    while (fread(&temp, sizeof(Student), 1, file_ptr) == 1) {
        if (temp.std_number == student_id && temp.deleted == 0) {
            temp.deleted = 1;
            fseek(file_ptr, -(sizeof(Student)), SEEK_CUR);
            size_t modified_student_written = fwrite(&temp, sizeof(Student), 1, file_ptr);
            if (modified_student_written == -1) {
                perror("Could not overwite the student data\n");
                fclose(file_ptr);
                return 1;
            }
            fclose(file_ptr);
            return 0;
        } else if (temp.std_number == student_id && temp.deleted == 1) {
            printf("Student already deleted\n");
            fclose(file_ptr);
            return 1;
        }
    }
    fclose(file_ptr);
    return 1;


}

int DeleteStudentByName(char *student_name) {
    // same as delete student by id except it deletes the student record as per their name
    FILE *file_ptr = fopen("student_record.dat", "rb+");
    if (file_ptr == NULL) {
        perror("Error opening the file\n");
        return 1;
    }
    Student temp;
    while (fread(&temp, sizeof(Student), 1, file_ptr) == 1) {
        if (strcmp(temp.std_name, student_name) == 0 && temp.deleted == 0) {
            temp.deleted = 1;
            fseek(file_ptr, -(sizeof(Student)), SEEK_CUR);
            size_t modified_student = fwrite(&temp, sizeof(Student), 1, file_ptr);
            if (modified_student == -1) {
                perror("Couldnt overwrite the student data\n");
                fclose(file_ptr);
                return 1;
            }
            fclose(file_ptr);
            return 0;
        } else if (strcmp(temp.std_name, student_name) == 0 && temp.deleted == 1) {
            printf("Student already deleted\n");
            fclose(file_ptr);
            return 1;
        }
    }
    fclose(file_ptr);
    return 1;
}

int UpdateStudentId(int student_id, int new_student_id) {
    // return 0 on success and 1 on a failure
    FILE *file_ptr = fopen("student_record.dat", "rb+");
    if (file_ptr == NULL) {
        perror("Error on opening file\n");
        return 1;
    }
    Student temp;
    while (fread(&temp, sizeof(Student), 1, file_ptr) == 1) {
        if (temp.std_number == student_id && temp.deleted == 0) {
            temp.std_number = new_student_id;
            fseek(file_ptr, -(sizeof(Student)), SEEK_CUR);
            size_t modified_student = fwrite(&temp, sizeof(Student), 1, file_ptr);
            if (modified_student == -1) {
                perror("couldnt overwrite the student data\n");
                fclose(file_ptr);
                return 1;
            }
            fclose(file_ptr);
            return 0;
        } else if (temp.std_number == student_id && temp.deleted == 1) {
            printf("Student could not be updated as record has been deleted\n");
            fclose(file_ptr);
            return 1;
        }
    }
    fclose(file_ptr);
    return 1;
}

int UpdateStudentName(char *student_name, char *new_name) {
    // return 0 on success and 1 on failure
    if (strlen(new_name) > 50) {
        printf("New name is too long\n");
        return 1;
    }
    FILE *file_ptr = fopen("student_record.dat", "rb+");
    if (file_ptr == NULL) {
        perror("Error opening the file\n");
        return 1;
    }
    Student temp;
    while (fread(&temp, sizeof(Student), 1, file_ptr) == 1) {
        if (strcmp(temp.std_name, student_name) == 0 && temp.deleted == 0) {
            // copy the new name into the field for the existing student name
            strncpy(temp.std_name, new_name, 50);
            temp.std_name[49] = '\0';
            fseek(file_ptr, -(sizeof(Student)), SEEK_CUR);
            size_t modified_student = fwrite(&temp, sizeof(Student), 1, file_ptr);
            if (modified_student == -1) {
                perror("Error when overwriting student data\n");
                fclose(file_ptr);
                return 1;
            }
            fclose(file_ptr);
            return 0;
        } else if (strcmp(temp.std_name, student_name) == 0 && temp.deleted == 1) {
            printf("Student already deleted\n");
            fclose(file_ptr);
            return 1;
        }
    }
    fclose(file_ptr);
    return 1;
}

int UpdateStudentProgram(int student_id, char *new_program) {
    // return 0 on success and 1 on failure

    FILE *file_ptr;
    file_ptr = fopen("student_record.dat", "rb+");
    if (file_ptr == NULL) {
        perror("Error opening the file\n");
        return 1;
    }
    Student temp;
    while (fread(&temp, sizeof(Student), 1, file_ptr) == 1) {
        if (temp.std_number == student_id && temp.deleted == 0) {
            strncpy(temp.std_program, new_program, 5);
            temp.std_program[4] = '\0';
            fseek(file_ptr, -(sizeof(Student)), SEEK_CUR);
            size_t modified_student = fwrite(&temp, sizeof(Student), 1, file_ptr);
            if (modified_student == -1) {
                perror("Error overwriting the student data\n");
                fclose(file_ptr);
                return 1;
            }
            fclose(file_ptr);
            return 0;
        } else if (temp.std_number == student_id && temp.deleted == 1) {
            printf("Student data has already been deleted\n");
            return 1;
        }
    }
}


