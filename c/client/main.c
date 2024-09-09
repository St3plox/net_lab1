#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define ADDRESS "127.0.0.1"
#define PORT 8080

int main(int argc, char const *argv[])
{
    int sock_fd;
    struct sockaddr_in server_addr;
    int received_number;

    // Create a TCP socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, ADDRESS, &server_addr.sin_addr) <= 0)
    {
        perror("Invalid address or Address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server.\n");

    int num;
    printf("Введите целое число \n");
    scanf("%d", &num);

    printf("Введите действие: \n 1 - извлечь корень \n 2 - возвести в квадрат \n");
    char action;
    scanf(" %c", &action);

    // Buffer: [char action, int num]
    unsigned char buffer[sizeof(char) + sizeof(int)];
    buffer[0] = action;

    int network_int_value = htonl(num);
    memcpy(buffer + sizeof(char), &network_int_value, sizeof(int));

    if (send(sock_fd, buffer, sizeof(buffer), 0) < 0)
    {
        perror("Failed to send data");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    printf("Sent action: %c and number: %d\n", action, num);

    // Receive the response from the server
    if (recv(sock_fd, buffer, sizeof(buffer), 0) < 0)
    {
        perror("Failed to receive data");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    memcpy(&received_number, buffer, sizeof(int));
    received_number = ntohl(received_number);

    printf("Received number: %d\n", received_number);

    // Close the socket
    close(sock_fd);

    return 0;
}
