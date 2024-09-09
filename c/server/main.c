#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080
#define BACKLOG 5

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int received_number, converted_number;
    int buffer;

    // Create a TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT); // Convert port number to network byte order

    // Bind the socket to the specified port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(server_fd, BACKLOG) < 0)
    {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    // Accept a client connection
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd < 0)
    {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Client connected.\n");

    // Receive an integer from the client
    if (recv(client_fd, &buffer, sizeof(buffer), 0) < 0)
    {
        perror("Failed to receive data");
        close(client_fd);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Convert received data from network byte order to host byte order
    received_number = ntohl(buffer);
    printf("Received number: %d\n", received_number);

    // code

    converted_number = htonl(received_number); // Convert back to network byte order

    // Send the number back to the client
    if (send(client_fd, &converted_number, sizeof(converted_number), 0) < 0)
    {
        perror("Failed to send data");
        close(client_fd);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Sent number back to the client.\n");

    // Close the sockets
    close(client_fd);
    close(server_fd);

    return 0;
}
