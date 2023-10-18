#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc , char* argv[]) {
    int client_socket;
    struct sockaddr_in server_address;

    // Create a socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating client socket");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(argv[1])); // Port number
    server_address.sin_addr.s_addr = INADDR_ANY; // Replace with the actual IP of Machine 1

    // Connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Error connecting to the server");
        close(client_socket);
        exit(EXIT_FAILURE); 
    }

    printf("Connected to server\n");
    char message[4096];
    while (1) {
        
        // bzero(message, 4096);
        memset(message, 0, sizeof(message));
        ssize_t bytes_received;

        // Receive the server's response
        bytes_received = recv(client_socket, message, sizeof(message),0);
        if (bytes_received <= 0) {
            break; // Connection closed by the server
        }

        // message[bytes_received] = '\0';
        printf("Server: %s", message);
    

        // char message[1024];
        // printf("");
        // char choice;
        memset(message, 0, sizeof(message));
         
        scanf(" %[^\n]" ,message);
        // (message, sizeof(message), stdin);
       
        // Send the message to the server
        send(client_socket, message, strlen(message),0);

        // if (strcmp(message, "exit\n") == 0) {
        //     break;
        // }

    }

    // Close the socket
    // printf()
    close(client_socket);

    return 0;
}
