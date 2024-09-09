#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    // Declare the necessary variables
    int sock_fd;
    struct sockaddr_in server_addr;
    int received_number;
    int buffer;

    // Create a TCP socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Specify the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080); // The port your server is running on

    // Convert the IP address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0)
    {
        perror("Invalid address or Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server.\n");

    int n = 123;
    // scanf("%d", &n);

    buffer = htonl(n);

    // Send the number to the server
    if (send(sock_fd, &buffer, sizeof(buffer), 0) < 0)
    {
        perror("Failed to send number");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    printf("Sent number: %d\n", n);

    // Receive the response from the server
    if (recv(sock_fd, &buffer, sizeof(buffer), 0) < 0)
    {
        perror("Failed to receive data");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    // Convert the received number from network byte order to host byte order
    received_number = ntohl(buffer);
    printf("Received number: %d\n", received_number);

    // Close the socket
    close(sock_fd);

    return 0;
}
