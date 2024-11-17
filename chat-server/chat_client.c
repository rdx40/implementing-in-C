#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h> // Include the pthread library

#define PORT 8080
#define BUFFER_SIZE 1024

// Function to handle receiving messages from the server
void *receive_messages(void *sock) {
    int server_socket = *(int *)sock;
    char buffer[BUFFER_SIZE];
    int bytes_read;

    while ((bytes_read = recv(server_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[bytes_read] = '\0'; // Null-terminate the string
        printf("\nServer: %s", buffer);
    }

    return NULL;
}

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char message[BUFFER_SIZE];
    pthread_t tid;

    // Create client socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Use localhost for testing

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server. Type messages to send:\n");

    // Create a thread to receive messages from the server
    if (pthread_create(&tid, NULL, receive_messages, (void *)&client_socket) != 0) {
        perror("Failed to create thread");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Loop to send messages
    while (1) {
        printf("You: ");
        fgets(message, BUFFER_SIZE, stdin);
        send(client_socket, message, strlen(message), 0);
    }

    close(client_socket);
    return 0;
}

