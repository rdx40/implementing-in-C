#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 3333
#define BUFFER_SIZE 1024

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    //create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0); //AFINET specifies IPv4 family, SOCK_STREAM specifies type of socket in this case tcp, 0 selects appropriate protocol for given socket type
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }



    //bind socket to ip and port
    server_addr.sin_family = AF_INET; //listen for incoming ipv4 
    server_addr.sin_addr.s_addr = INADDR_ANY; // to accept all connections
    server_addr.sin_port = htons(PORT); //specifies port on which server listens //htons() has something to do with byte storage order especially on little endian archs

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }


    
    //listen for incoming connections
    if (listen(server_socket, 3) == -1) { 
	   //socket in listen state and for max pending queue of 3 
        perror("Listening failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d...\n", PORT);



    // accept connection from client
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_socket == -1) {
        perror("Acceptance failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    printf("Client connected.\n");

    
    
    //receive data from the client
    memset(buffer, 0, BUFFER_SIZE); //memset clears buffer before recieving any data
    ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received == -1) {
	    perror("Receiving failed");
    }else {
	    buffer[bytes_received] = '\0'; // Null-terminate the received data
	    printf("Received data: %s\n", buffer);
    }

    //close the connection
    close(client_socket);
    close(server_socket);
    printf("Connection closed.\n");

    return 0;
}
