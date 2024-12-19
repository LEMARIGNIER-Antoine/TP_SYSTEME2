#ifndef GETTFTP_H
#define GETTFTP_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024
#define DATA_SIZE 512 // Taille maximale des données dans un paquet DATA

void main(int argc, char **argv);

#endif //GETTFTP_H
