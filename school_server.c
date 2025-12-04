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

// Helper Functions
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
    // add an if block to ensure where to put the null terminator
    if (i == 5) {
        temp_buff[4] = '\0';
    } else if (i <= 4) {
        temp_buff[i] = '\0';
    }
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

void handle_add_student(int connection) {
    char buf[1024];
    const char *prompt_id = "AddStudent selected.\nEnter student number:\n";
    write(connection, prompt_id, strlen(prompt_id));

    if (read_line(connection, buf, sizeof(buf)) <= 0) {
        return;
    }
    int student_id = atoi(buf);

    // Ask for name
    const char *prompt_name = "Enter student name:\n";
    write(connection, prompt_name, strlen(prompt_name));

    if (read_line(connection, buf, sizeof(buf)) <= 0) {
        return;
    }
    char student_name[50];
    strncpy(student_name, buf, 50);
    student_name[49] = '\0';

    // Ask for age
    const char *prompt_age = "Enter student age:\n";
    write(connection, prompt_age, strlen(prompt_age));

    if (read_line(connection, buf, sizeof(buf)) <= 0) {
        return;
    }
    int student_age = atoi(buf);

    // Ask for program
    const char *prompt_program = "Enter student program (max 4 chars):\n";
    write(connection, prompt_program, strlen(prompt_program));

    if (read_line(connection, buf, sizeof(buf)) <= 0) {
        return;
    }
    char program[5];
    strncpy(program, buf, 5);
    program[4] = '\0';

    // Now we have everything: call your existing function
    int rc = AddStudent(student_id, student_name, student_age, program);
    if (rc == 0) {
        const char *ok = "Student added successfully.\n";
        write(connection, ok, strlen(ok));
    } else {
        const char *err = "Error adding student.\n";
        write(connection, err, strlen(err));
    }
}

void handle_find_student_id(int connection) {
    char buff[1024];
    char prompt_id[] = "Find Student by ID selected.\nEnter Student ID:\n";
    write(connection, prompt_id, strlen(prompt_id));
    if (read_line(connection, buff, sizeof(buff)) <= 0) {
        return;
    }
    int student_id = atoi(buff);
    Student *mem_alloc = malloc(sizeof(Student));
    int ret_val = FindStudentId(student_id, mem_alloc);
    if (ret_val == 1) {
        char error_message[] = "Error in file opening\n";
        write(connection, error_message, strlen(error_message));
        free(mem_alloc);
        return;
    } else if (ret_val == 2) {
        char not_found_message[] = "No Student Found\n";
        write(connection, not_found_message, strlen(not_found_message));
        free(mem_alloc);
        return;
    }
    char student_number[11];
    char student_age[3];
    char student_name[50];
    char student_program[5];
    sprintf(student_number, "%d" ,(*mem_alloc).std_number );
    sprintf(student_age, "%d", (*mem_alloc).std_age);
    strncpy(student_name, (*mem_alloc).std_name, 50);
    student_name[49] = '\0';
    strncpy(student_program, (*mem_alloc).std_program, 5);
    student_program[4] = '\0';
    write(connection, student_number, strlen(student_number));
    write(connection, student_age, strlen(student_age));
    write(connection, student_name, strlen(student_name));
    write(connection, student_program, strlen(student_program));
}

void handle_find_student_name(int connection) {
    char buff[1024];
    char prompt_name[] = "Find Student By Name Selected.\nEnter Student Name\n";
    write(connection, prompt_name, strlen(prompt_name));
    if (read_line(connection, buff, strlen(buff)) <= 0) {
        return;
    }
    Student *curr_student = malloc(sizeof(Student));
    int return_value = FindStudentName(buff, curr_student);
    if (return_value == 1) {
        char err_message[] = "Error in Opening Data File.\n";
        write(connection, err_message, strlen(err_message));
        free(curr_student);
        return;
    }
    if (return_value == 2) {
        char not_found_message[] = "Student not Found.\n";
        write(connection, not_found_message, strlen(not_found_message));
        free(curr_student);
        return;
    }
    char student_number[11];
    char student_age[3];
    char student_name[50];
    char student_program[5];
    sprintf(student_number, "%d" ,(*curr_student).std_number );
    sprintf(student_age, "%d", (*curr_student).std_age);
    strncpy(student_name, (*curr_student).std_name, 50);
    student_name[49] = '\0';
    strncpy(student_program, (*curr_student).std_program, 5);
    student_program[4] = '\0';
    write(connection, student_number, strlen(student_number));
    write(connection, student_age, strlen(student_age));
    write(connection, student_name, strlen(student_name));
    write(connection, student_program, strlen(student_program));

}

void handle_students_in_program(int connection) {
    char temp_buff[1024];
    int student_count;
    char prompt_program[] = "Students in Program selected.\nEnter Program to check.\n";
    write(connection, prompt_program, strlen(prompt_program));
    if (read_line(connection, temp_buff, strlen(temp_buff)) <= 0) {
        return;
    }
    Student *heap_ptr = StudentsInProgram(temp_buff, &student_count);
    int i = 0;
    while (i < student_count) {
        char student_number[11];
        char student_age[3];
        char student_name[50];
        sprintf(student_number, "%d", heap_ptr[i].std_number);
        sprintf(student_age, "%d", heap_ptr[i].std_age);
        strncpy(student_name, heap_ptr[i].std_name, 50);
        student_name[49] = '\0';
        char message[150];
        strncat(message, student_number, sizeof(message) - strlen(message) - 1);
        strncat(message, " ", sizeof(message) - strlen(message) - 1);
        strncat(message, student_age, sizeof(message) - strlen(message) - 1);
        strncat(message, " ", sizeof(message) - strlen(message) - 1);
        strncat(message, student_name, sizeof(message) - strlen(message) - 1);
        strncat(message, "\n", sizeof(message) - strlen(message) - 1);
        write(connection, message, strlen(message));
        i++;
    }
}

void handle_total_students(int connection) {
    char buff[1024];
    char *message = "Total Students selected.";
    write(connection, message, strlen(message));
    if (read_line(connection, buff, strlen(buff)) <= 0) {
        return;
    }
    int total_students = TotalStudents();
    char students_pop[6];
    sprintf(students_pop, "%d", total_students);
    char response[150];
    response[0] = '\0';
    char *final_message = "Total Students at the school: ";
    strncat(response, final_message, sizeof(response) - strlen(response) - 1);
    strncat(response, students_pop, sizeof(response) - strlen(response) - 1);
    write(connection, response, strlen(response));
}

void handle_delete_student_by_id(int connection) {
    char buff[1024];
    char *prompt = "Delete Student By Id function selected.\nEnter Student ID: \n";
    write(connection, prompt, strlen(prompt));
    if (read_line(connection, buff, sizeof(buff)) <= 0) {
        return;
    }
    int student_id = atoi(buff);
    int return_value = DeleteStudentById(student_id);
    if (return_value != 0) {
        char *error_message = "An error occurred.\nUnable to delete student id.\n";
        write(connection, error_message, strlen(error_message));
        return;
    }
    char *success_message = "Student data deleted.\n";
    write(connection, success_message, strlen(success_message));
}

void handle_delete_student_by_name(int connection) {
    char buff[1024];
    char *prompt = "Delete Student By Name selected.\nEnter Student Name: \n";
    write(connection, prompt, strlen(prompt));
    if (read_line(connection, buff, sizeof(buff)) <= 0) {
        return;
    }
    int return_value = DeleteStudentByName(buff);
    if (return_value != 0) {
        char *error_message = "An error occurred.\nUnable to delete student record.\n";
        write(connection, error_message, strlen(error_message));
        return;
    }
    char *success_manage = "Student data deleted.\n";
    write(connection, success_manage, strlen(success_manage));
}

void handle_update_student_id(int connection) {
    char first_response[50];
    char second_response[50];
    char *prompt = "Update Student by Id function selected.\nEnter current student id: \n";
    write(connection, prompt, strlen(prompt));
    if (read_line(connection, first_response, sizeof(first_response)) <= 0) {
        return;
    }
    char *second_prompt = "Enter new student id: \n";
    write(connection, second_prompt, strlen(second_prompt));
    if (read_line(connection, second_response, sizeof(second_response)) <= 0) {
        return;
    }
    int old_student_id = atoi(first_response);
    int new_student_id = atoi(second_response);
    int return_value = UpdateStudentId(old_student_id, new_student_id);
    if (return_value != 0) {
        char *error_message = "Error occurred.\nUnable to update student record.\n";
        write(connection, error_message, strlen(error_message));
        return;
    }
    char *success_message = "Student record updated.\n";
    write(connection, success_message, strlen(success_message));
}

void handle_update_student_name(int connection) {
    char first_response[100];
    char second_response[100];
    char *first_prompt = "Update Student by Name function selected.\nEnter current student name: \n";
    write(connection, first_prompt, strlen(first_prompt));
    if (read_line(connection, first_response, sizeof(first_response)) <= 0) {
        return;
    }
    char *second_prompt = "Enter new student name: \n";
    write(connection, second_prompt, strlen(second_prompt));
    if (read_line(connection, second_response, sizeof(second_response)) <= 0) {
        return;
    }
    int return_value = UpdateStudentName(first_response, second_response);
    if (return_value != 0) {
        char *error_message = "An error occurred.\nUnable to update student record.\n";
        write(connection, error_message, strlen(error_message));
        return;
    }
    char *success_message = "Student record updated.\n";
    write(connection, success_message, strlen(success_message));
}

void handle_update_student_program(int connection) {
    char first_response[50];
    char second_response[10];
    char *prompt = "Update Student Name function selected.\nEnter Student id: \n";
    write(connection, prompt, strlen(prompt));
    if (read_line(connection, first_response, sizeof(first_response)) <= 0) {
        return;
    }
    char *second_prompt = "Enter new program: \n";
    write(connection, second_prompt, strlen(second_prompt));
    if (read_line(connection, second_response, sizeof(second_response)) <= 0) {
        return;
    }
    int student_id = atoi(first_response);
    int return_value = UpdateStudentProgram(student_id, second_response);
    if (return_value != 0) {
        char *error_message = "Error occurred.\nUnable to update student program.\n";
        write(connection, error_message, strlen(error_message));
        return;
    }
    char *success_message = "Student Record updatd.\n";
    write(connection, success_message, strlen(success_message));
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
        // char response[1024];
        ssize_t bytes_received = read(connection, buffer, sizeof(buffer));
        if (bytes_received == -1) {
            perror("Error in client message\n");
        } else if (bytes_received == 0) {
            perror("Server disconnected. Server will close. Restart the server to continue\n");
            break;
        }
        buffer[bytes_received] = '\0';
        if (strcmp(buffer, "Q") == 0 || strcmp(buffer, "quit") == 0 || strcmp(buffer, "q") == 0) {
            break;
        }
        char func_code[5];
        int return_value = GetServerFunction(buffer, func_code);
        if (return_value != -1){
            if (strcmp(func_code, "AS") == 0) {
                handle_add_student(connection);
            } else if (strcmp(func_code, "FSI") == 0) {
                handle_find_student_id(connection);
            } else if (strcmp(func_code, "FSN") == 0) {
                handle_find_student_name(connection);
            } else if (strcmp(func_code, "SIP") == 0) {
                handle_students_in_program(connection);
            } else if (strcmp(func_code, "TS") == 0){
                handle_total_students(connection);
            } else if (strcmp(func_code, "DSBI") == 0) {
                handle_delete_student_by_id(connection);
            } else if (strcmp(func_code, "DSBN") == 0) {
                handle_delete_student_by_name(connection);
            } else if (strcmp(func_code, "USI") == 0) {
                handle_update_student_id(connection);
            } else if (strcmp(func_code, "USN") == 0) {
                handle_update_student_name(connection);
            } else if (strcmp(func_code, "USP") == 0) {
                handle_update_student_program(connection);
            }
        } else {
            char *error_message = "Incorrect function code selected.\n";
            write(connection, error_message, strlen(error_message));
        }

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
        Student *heap_ptr = malloc(set_count * sizeof(Student));
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


