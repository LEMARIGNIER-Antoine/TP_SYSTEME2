cmake_minimum_required(VERSION 3.30)
project(clientTFTP C)

set(CMAKE_C_STANDARD 11)

add_executable(clientTFTP
        gettftp.c
        gettftp.h)
