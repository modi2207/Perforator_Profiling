#include <iostream>
#include <cstring>
#include <cctype>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 12000
#define BUFFER_SIZE 1024

void to_uppercase(char* str) {
    while (*str) {
        *str = std::toupper(*str);
        ++str;
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attach socket to the port 12000
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    address.sin_port = htons(PORT);

    // Bind the socket to the port
    if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 10) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "The server is ready to receive\n";

    while (true) {
        // Accept a new connection
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address,
                                 (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            continue;
        }

        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = read(new_socket, buffer, BUFFER_SIZE);

        if (bytes_read > 0) {
            std::cout << "Received: " << buffer << std::endl;

            // Capitalize the sentence
            to_uppercase(buffer);

            // Sleep for 10 seconds//
//           sleep(1);

            // Send back capitalized sentence
            send(new_socket, buffer, strlen(buffer), 0);
        }

        close(new_socket);
    }

    return 0;
}
