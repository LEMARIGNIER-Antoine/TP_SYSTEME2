#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "gettftp.h"

#define BUFFER_SIZE 1024
#define DATA_SIZE 512 // Taille maximale des données dans un paquet DATA

void main(int argc, char **argv) {  // Changement du type de retour à void
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <host> <file>\n", argv[0]);
        return;  // Retour sans valeur
    }

    printf("**** Name resolution for %s ****\n", argv[1]);

    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_protocol = IPPROTO_UDP; // TFTP utilise UDP
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_DGRAM;  // Socket UDP

    struct addrinfo *res;
    if (getaddrinfo(argv[1], "1069", &hints, &res) != 0) { // we are gonna use the port 1069 and see it with wireshark
        perror("getaddrinfo");
        return;
    }

    int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sd < 0) {
        perror("socket");
        freeaddrinfo(res);
        return;
    }

    // Build the RRQ request
    char request[BUFFER_SIZE];
    memset(request, 0, BUFFER_SIZE);

    // 1. Add the opcode (01 for RRQ)
    request[0] = 0;  // First byte of the opcode
    request[1] = 1;  // Second byte of the opcode (01 for RRQ)

    // 2. Add the filename
    const char *filename = argv[2]; // Filename passed as an argument
    strcpy(&request[2], filename);

    // 3. Add the null byte after the filename
    int offset = 2 + strlen(filename) + 1;

    // 4. Add the transfer mode (e.g., "octet")
    const char *mode = "octet"; // Binary mode for TFTP
    strcpy(&request[offset], mode);

    // 5. Add the null byte after the transfer mode
    offset += strlen(mode) + 1;

    // 6. Final size of the request
    int request_size = offset;

    // Send the request to the server
    ssize_t req = sendto(sd, request, request_size, 0, res->ai_addr, res->ai_addrlen);
    printf("Request size: %d\n", req);

    // Wait for the DATA packets from the server
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);

    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("fopen");
        close(sd);
        return;
    }

    unsigned short expected_block = 1; // Block number we expect to receive
    int end_of_file = 0;

    do {
        ssize_t len = recvfrom(sd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &addr_len);

        // Check if the received packet is a DATA packet (opcode 3)
        if (buffer[0] == 0 && buffer[1] == 3) {
            // Extract the block number from the packet (bytes 2 and 3)
            unsigned short block_num = ntohs(*(unsigned short *)&buffer[2]);

            if (block_num == expected_block) {
                printf("Received DATA packet, block number: %d\n", block_num);

                // Write the data into the file
                fwrite(&buffer[4], 1, len - 4, file);  // Skip the opcode and block number, write data

                // Check if this is the last packet (less than 512 bytes of data)
                if (len - 4 < DATA_SIZE) {
                    end_of_file = 1; // End of file detected
                }

                // Send an ACK to acknowledge the received DATA packet
                char ack[4];
                ack[0] = 0;
                ack[1] = 4;  // Opcode for ACK
                *(unsigned short *)&ack[2] = htons(block_num);  // Block number

                ssize_t ack_len = sendto(sd, ack, sizeof(ack), 0, (struct sockaddr *)&server_addr, addr_len);
                if (ack_len < 0) {
                    perror("sendto ACK");
                    break;
                }
                printf("ACK sent for block number %d\n", block_num);

                // Increment the expected block number
                expected_block++;
            } else {
                printf("Unexpected block number: %d (expected %d)\n", block_num, expected_block);
            }
        } else {
            fprintf(stderr, "Received invalid packet, expected DATA packet\n");
            break;
        }
    } while (!end_of_file);

    fclose(file);
    close(sd);
    printf("File transfer complete.\n");
}
