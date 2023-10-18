#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "admin.h"
#include "faculty.h"
#include "student.h"
// int port = 8000;
#define MAX_CLIENTS 10


void handle_admin(int client_socket){
    char username[4096];
    char password[4096];
    // printf("enter username");
    char buf_un[] = "Enter Username: ";
    send(client_socket , buf_un, strlen(buf_un),0);

    int bytes_read = recv(client_socket, username, sizeof(username),0);
    if(bytes_read == -1){
        perror("username read failed in server");
    }

    char buf_ps[]= "Enter Password : ";
    send(client_socket ,buf_ps , strlen(buf_ps),0);

    int ps_read = recv(client_socket, password, sizeof(password),0);
    if(ps_read == -1){
        perror("read failed in server");
    }
    // printf("\nusername: %ld\n",strlen(username));
    if(!strcmp(username , "Harsh") && !strcmp(password , "Harsh123")){
        printf("Welcome admin , %s \n" , username);
        send_menu(client_socket);
    }
    else{
        printf("Entered username and password are not correct \n" );
    }

    
    

}


void handle_client(int client_socket) {
    // Handle the client connection here
    // You can read and write data from/to the client socket
    // Example: echo back received data
    // while(1){
    char buffer[1024];
    ssize_t bytes_received;
    char login_roles[4096] = "<--------------------welcome to Academia :: Best platform for course registration ------------------------->\n"
    "Login as :\n"
    "Enter your choice : [1 : Admin , 2 : Faculty, 3 : Student] \n";

    

    send(client_socket, login_roles, strlen(login_roles) , 0);

    // char cho ice[2];
    // while(1){
         int choice_bytes = recv(client_socket, &buffer, sizeof(buffer),0);
         if(choice_bytes <= 0){
            printf("enter valid choice");
         }
        printf("choice entered by client: %s\n", buffer);
        
        // if(atoi(buffer) == 4){
        //     break;
        //     return;
        // }

        // Echo the data back to the client
        // send(client_socket, buffer, bytes_received, 0);
    // }

    switch(atoi(buffer)){
    case 1: printf("inplementation of Admin");
        handle_admin(client_socket);
        break;
    case 2: printf("inplementation of Faculty");
        handle_faculty_auth(client_socket);
        break;
    case 3: printf("inplementation of Student");
        handle_student_auth(client_socket);
        break;
    default : printf("Entered choice is invalid");
    }


    close(client_socket);
    // }
  
}

// void get_login(){

// }



int main(int argc , char* argv[]) {
    int server_socket, client_socket, addr_len;
    struct sockaddr_in server_addr, client_addr;

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Server socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(1);
    }

    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("Listen failed");
        exit(1);
    }

    printf("Server listening on port %d...\n", atoi(argv[1]));

    while (1) {
        addr_len = sizeof(struct sockaddr_in);
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t *)&addr_len)) == -1) {
            perror("Accept failed");
            continue;
        }

        if (fork() == 0) {  // Child process
            close(server_socket);  // Child doesn't need the listening socket
            handle_client(client_socket);
            exit(0);
        } else {
            close(client_socket);  // Parent doesn't need the client socket
        }
    }

    close(server_socket);

    return 0;
}

