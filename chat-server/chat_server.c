#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024

// Array to keep track of connected clients
int client_sockets[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *client_socket) {
    int sock = *(int *)client_socket;
    char buffer[BUFFER_SIZE];
    int bytes_read;

    // Notify connection
    printf("Client connected on socket %d\n", sock);

    while ((bytes_read = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[bytes_read] = '\0'; // Null-terminate the string

        // Broadcast the received message to all other clients
        pthread_mutex_lock(&clients_mutex);
        for (int i = 0; i < client_count; ++i) {
            if (client_sockets[i] != sock) {
                send(client_sockets[i], buffer, bytes_read, 0);
            }
        }
        pthread_mutex_unlock(&clients_mutex);
    }

    // Remove the client socket from the list and close it
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; ++i) {
        if (client_sockets[i] == sock) {
            for (int j = i; j < client_count - 1; ++j) {
                client_sockets[j] = client_sockets[j + 1];
            }
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    close(sock);
    printf("Client disconnected from socket %d\n", sock);
    free(client_socket);
    return NULL;
}

int main() {
    int server_socket, new_client;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 10) < 0) {
        perror("Listening failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server started. Waiting for clients on port %d...\n", PORT);

    while (1) {
        new_client = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
        if (new_client < 0) {
            perror("Accept failed");
            continue;
        }

        pthread_mutex_lock(&clients_mutex);
        if (client_count < MAX_CLIENTS) {
            client_sockets[client_count++] = new_client;
            pthread_t tid;
            int *client_sock_ptr = malloc(sizeof(int));
            *client_sock_ptr = new_client;
            pthread_create(&tid, NULL, handle_client, client_sock_ptr);
        } else {
            printf("Maximum clients reached. Connection rejected.\n");
            close(new_client);
        }
        pthread_mutex_unlock(&clients_mutex);
    }

    close(server_socket);
    return 0;
}

