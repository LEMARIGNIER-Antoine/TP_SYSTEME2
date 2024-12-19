//
// Created by grimont on 11/12/24.
//

#ifndef PUTTFTP_H
#define PUTTFTP_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#define BUFFER_SIZE 1024
#define DATA_SIZE 512 // Maximum data size for a DATA packet

void main(int argc, char **argv);
#endif //PUTTFTP_H
