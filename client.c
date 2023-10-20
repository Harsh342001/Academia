#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>

void conn(int sockFD);

void main()
{
    int socketFileDescriptor, connectStatus;
    struct sockaddr_in serverAddress;
    struct sockaddr server;

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

    connectStatus = connect(socketFileDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (connectStatus == -1)
    {
        perror("Error while connecting to server!");
        close(socketFileDescriptor);
        _exit(0);
    }

    conn(socketFileDescriptor);

    close(socketFileDescriptor);
}


void conn(int sockFD)
{
    char readBuffer[1000], writeBuffer[1000];
    ssize_t readBytes, writeBytes;

    char tempBuffer[1000];

    
    while(1){
        bzero(readBuffer, 1000);
        // bzero(tempBuffer, sizeof(tempBuffer));
        readBytes = read(sockFD, readBuffer, sizeof(readBuffer));
        // if(readBytes==0)break;
		// printf("read bytes: %ld\n",readBytes);
		// printf("rbuf: %s\n",readBuffer);
        if(strcmp(readBuffer, "Invalid Choice!\n")==0)break;

        else if (readBytes == -1)
            perror("Error while reading from client socket!");
        else if (readBytes == 0)
            printf("No error received from server! Closing the connection to the server now!\n");
        
        else
        {
            bzero(writeBuffer, 1000); // Empty the write buffer

			// printf("%s\n", readBuffer);
            write(1, readBuffer, sizeof(readBuffer));
            int i=0;
            while(1){
                char c;
                // fflush(stdin);
                read(0, &c, 1);
                if(c=='\n')break;
                writeBuffer[i]=c;
                i++;
            }

            writeBuffer[i]='\0';
            

            writeBytes = write(sockFD, writeBuffer, strlen(writeBuffer));

            if (writeBytes == -1)
            {
                perror("Error while writing to client socket!");
                printf("Closing the connection to the server now!\n");
                break;
            }
        }
    }

    close(sockFD);

}


