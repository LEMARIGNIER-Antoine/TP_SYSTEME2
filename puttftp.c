//
// Created by grimont on 11/12/24.
//
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#include "puttftp.h"

int puttftp(int argc, char ** argv) {
    printf("**** Name resolution for %s ****\n", argv[1]);
    struct addrinfo hints;
    memset(&hints,0,sizeof(struct addrinfo));
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_family = AF_INET;//IPV4

    struct addrinfo * res;
    getaddrinfo(argv[1],80,&hints,&res); // obtain the address of the serveur


}