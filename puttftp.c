#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024
#define DATA_SIZE 512 // Maximum data size for a DATA packet

void main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <host> <file>\n", argv[0]);
        return;
    }

    printf("**** Name resolution for %s ****\n", argv[1]);

    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_protocol = IPPROTO_UDP; // TFTP uses UDP
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_DGRAM;  // UDP socket

    struct addrinfo *res;
    if (getaddrinfo(argv[1], "1069", &hints, &res) != 0) { // Port 1069 to observe with Wireshark
        perror("getaddrinfo");
        return;
    }

    int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sd < 0) {
        perror("socket");
        freeaddrinfo(res);
        return;
    }

    // ** Build the WRQ request **
    char request[BUFFER_SIZE];
    memset(request, 0, BUFFER_SIZE);

    // 1. Add the opcode (02 for WRQ)
    request[0] = 0;  // First byte of the opcode
    request[1] = 2;  // Second byte of the opcode (02 for WRQ)

    // 2. Add the filename
    const char *filename = argv[2];
    strcpy(&request[2], filename);

    // 3. Add the null byte after the filename
    int offset = 2 + strlen(filename) + 1;

    // 4. Add the transfer mode (octet)
    const char *mode = "octet";
    strcpy(&request[offset], mode);

    // 5. Add the null byte after the mode
    offset += strlen(mode) + 1;

    // Final size of the WRQ request
    int request_size = offset;

    // ** Print the request packet for debugging **
    printf("WRQ Request Packet:\n");
    for (int i = 0; i < request_size; i++) {
        printf("%02x ", (unsigned char)request[i]);
    }
    printf("\n");

    // ** Write the WRQ request to a file **
    FILE *file = fopen("wrq_request.txt", "wb");
    if (file == NULL) {
        perror("fopen");
        close(sd);
        freeaddrinfo(res);
        return;
    }

    if (fwrite(request, 1, request_size, file) != request_size) {
        perror("fwrite");
        fclose(file);
        close(sd);
        freeaddrinfo(res);
        return;
    }

    fclose(file);
    printf("WRQ request saved to 'wrq_request.txt'.\n");

    // ** Send the WRQ request to the server **
    ssize_t req = sendto(sd, request, request_size, 0, res->ai_addr, res->ai_addrlen);
    if (req < 0) {
        perror("sendto WRQ");
        close(sd);
        return;
    }
    printf("WRQ request sent. Request size: %d bytes\n", req);

    // ** Wait for the ACK for the WRQ (block 0) **
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);

    ssize_t len = recvfrom(sd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &addr_len);
    if (len > 0 && buffer[0] == 0 && buffer[1] == 4) { // Opcode 04 for ACK
        unsigned short block_num = ntohs(*(unsigned short *)&buffer[2]);
        if (block_num == 0) {
            printf("ACK received for WRQ (block number: %d)\n", block_num);
        } else {
            fprintf(stderr, "Unexpected ACK block number: %d\n", block_num);
            close(sd);
            return;
        }
    } else {
        fprintf(stderr, "Failed to receive ACK for WRQ.\n");
        close(sd);
        return;
    }

    printf("WRQ handshake complete. Ready to send data.\n");

    // Close the socket
    close(sd);
}
