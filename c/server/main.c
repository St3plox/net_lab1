#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#define BACKLOG 5
#define PORT 8080

int int_sqrt(int number);

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, BACKLOG) < 0)
    {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port 8080...\n");

    // Accept a connection
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0)
    {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Client connected.\n");

    // Buffer: [char action, int num]
    unsigned char buffer[sizeof(char) + sizeof(int)];

    if (recv(client_fd, buffer, sizeof(buffer), 0) < 0)
    {
        perror("Receive failed");
        close(client_fd);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    char action = buffer[0];
    int number;
    memcpy(&number, buffer + sizeof(char), sizeof(int));
    number = ntohl(number);

    int res;
    if (action == '1')
    {
        res = int_sqrt(number);
        if (res < 0)
        {
            perror("Sqrt failed");
            close(client_fd);
            close(server_fd);
            exit(EXIT_FAILURE);
        }
    }
    else if (action == '2')
    {
        res = number * number;
    }
    else
    {
        perror("Unsupported operation");
        close(client_fd);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Received char: '%c' and int: %d\n", action, number);

    // Send the result back to the client
    int network_res = htonl(res); // Convert result to network byte order
    if (send(client_fd, &network_res, sizeof(int), 0) < 0)
    {
        perror("Send failed");
        close(client_fd);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Close the sockets
    close(client_fd);
    close(server_fd);

    return 0;
}

int int_sqrt(int number)
{
    if (number < 0)
    {
        return -1;
    }

    double sqrt_result = sqrt((double)number);
    return (int)round(sqrt_result);
}
