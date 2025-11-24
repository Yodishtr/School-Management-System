cmake_minimum_required(VERSION 4.0)
project(School_Management_System C)

set(CMAKE_C_STANDARD 11)

add_executable(School_Management_System
        school_server.c
        school_server_client.c)
