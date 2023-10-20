#include <stdio.h>
#include <errno.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>

#include <string.h> 
#include <stdbool.h> 
#include <stdlib.h> 

#include "auth.h"
#include "student.h"
#include "faculty.h"
#include "admin.h"

void conn(int connFD);

void main()
{
    int socketFileDescriptor, socketBindStatus, socketListenStatus, connectionFileDescriptor;
    struct sockaddr_in serverAddress, clientAddress;

    socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFileDescriptor == -1)
    {
        perror("Error while creating server socket!");
        _exit(0);
    }

    printf("Enter the port number: ");
    int port;
    scanf("%d",&port);

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = htons(INADDR_ANY); 

    socketBindStatus = bind(socketFileDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (socketBindStatus == -1)
    {
        perror("Error while binding to server socket!");
        _exit(0);
    }

    socketListenStatus = listen(socketFileDescriptor, 10);
    if (socketListenStatus == -1)
    {
        perror("Error while listening for connections on the server socket!");
        close(socketFileDescriptor);
        _exit(0);
    }

    int clientSize;
    while (1)
    {
        clientSize = (int)sizeof(clientAddress);
        connectionFileDescriptor = accept(socketFileDescriptor, (struct sockaddr *)&clientAddress, &clientSize);
        if (connectionFileDescriptor == -1)
        {
            perror("Error while connecting to client!");
            close(socketFileDescriptor);
        }
        else
        {
            if (!fork())
            {
                // Child will enter this branch
                conn(connectionFileDescriptor);
                close(connectionFileDescriptor);
                // _exit(0);
                break;
            }
			
        }
    }

    close(socketFileDescriptor);
}

void conn(int cfd)
{
    printf("Client has connected to the server!\n");

    bool flag=0;
    while(1){
        char readBuffer[1000], writeBuffer[1000];
        ssize_t readBytes, writeBytes;
        int userChoice;

        strcpy(writeBuffer, "....................................Welcome Back to Academia :: Course Registration....................................\n\nEnter your role {1. Faculty 2. Student 3. Admin 4. Exit}\n");

        writeBytes = write(cfd, writeBuffer, strlen(writeBuffer));

        if (writeBytes == -1)
            perror("Error while sending first prompt to the user!");
        else
        {
            bzero(readBuffer, 1000);
            readBytes = read(cfd, readBuffer, sizeof(readBuffer));

            

            if (readBytes == -1)
                perror("Error while reading from client");
            else if (readBytes == 0)
                printf("No data was sent by the client");
            else
            {
                userChoice = atoi(readBuffer);
                printf("%d\n",userChoice);
                switch (userChoice)
                {
                case 1:
                    facultyMenu(cfd);
                    break;
                case 2:
                    studentMenu(cfd);
                    break;
                case 3:
                    adminMenu(cfd);
                    break;
                default:
                    memset(writeBuffer, 0, sizeof(writeBuffer));
                    strcpy(writeBuffer, "Invalid Choice!\n");
                    write(cfd, writeBuffer, strlen(writeBuffer));
                    flag=1;
                    break;
                }
            }
        }
        if(flag)break;
    }
    printf("Terminating connection to client!\n");
}
