#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1" //localhost
#define PORT 3333
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    //connect to server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
	   //converts ip from string to binary format used in cn
        perror("Invalid address/Address not supported");
        close(client_socket);
        exit(EXIT_FAILURE);
    }


    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    printf("Connected to server.\n");

    //send data to the server
    printf("Enter data to send: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; 
    ssize_t bytes_sent = send(client_socket, buffer, strlen(buffer), 0);
    if (bytes_sent == -1) {
        perror("Sending failed");
    } else {
        printf("Data sent: %s\n", buffer);
    }

    //close the connection
    close(client_socket);
    printf("Connection closed.\n");

    return 0;
}
