#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "structs.h"
// #include "helper.h"

// student login
bool studentLogin(int cfd, struct Student *s)
{

        char user[1024], pass[1024];
        // ask for username
        struct Student student;
        // student.rollno=-1;
        memset(user, 0, sizeof(user));
        memset(user, 0, sizeof(user));

        int wb = write(cfd, "Enter username: ", 16);
        if (wb == -1)
        {
                perror("Error while asking for username: ");
                return false;
        }

        int rb = read(cfd, user, sizeof(user));
        
        if (rb == -1)
        {
                perror("Error while reading username: ");
                return false;
        }
        user[rb]='\0';
        // ask for password

        wb = write(cfd, "Enter password: ", 16);
        if (wb == -1)
        {
                perror("Error while asking for password: ");
                return false;
        }

        rb = read(cfd, pass, sizeof(pass));

        if (rb == -1)
        {
                perror("Error while reading password: ");
                return false;
        }       
        pass[rb]='\0';
        int fd = open("students.txt", O_RDONLY | O_CREAT);

        

        if (fd == -1)
        {
                perror("Error opening file");
                return false;
        }

        

        while (read(fd, &student, sizeof(struct Student)) > 0)
        {
                // printf("id: %d\n",student.rollno);
                // printf("user: %s\n",student.username);
                // printf("pass: %s\n",student.password);
                if (strcmp(student.username, user) == 0 && strcmp(student.password, pass) == 0)
                {
                        *s = student;
                        close(fd);
                        return true;
                }
        }

        close(fd);
        // student.rollno=-1;
        return false;

}

// faculty login
bool facultyLogin(int cfd, struct Faculty *f)
{

        char user[50], pass[50];
        // ask for username
        memset(user, 0, sizeof(user));
        memset(user, 0, sizeof(user));


        int wb = write(cfd, "Enter username: ", 16);
        if (wb == -1)
        {
                perror("Error while asking for username: ");
                return false;
        }

        int rb = read(cfd, user, sizeof(user));

        user[rb]='\0';
        // printf("username: %s\n",user);
        if (rb == -1)
        {
                perror("Error while reading username: ");
                return false;
        }
        // ask for password

        wb = write(cfd, "Enter password: ", 16);
        if (wb == -1)
        {
                perror("Error while asking for password: ");
                return false;
        }

        rb = read(cfd, pass, sizeof(pass));
        pass[rb]='\0';
        // printf("password: %s\n",pass);
        if (rb == -1)
        {
                perror("Error while reading password: ");
                return false;
        }

        int fd = open("faculty.txt", O_RDONLY | O_CREAT);


        if (fd == -1)
        {
                perror("Error opening file");
                return false;
        }

        struct Faculty faculty;

        while (read(fd, &faculty, sizeof(struct Faculty)) > 0)
        {
                // printf("id: %d\n",faculty.faculty_id);
                // printf("user: %s\n",faculty.username);
                // printf("pass: %s\n",faculty.password);
                if (strcmp(faculty.username, user) == 0 && strcmp(faculty.password, pass) == 0)
                {
                        *f = faculty;
                        close(fd);
                        return true;
                }
        }

        close(fd);
        return false;
}



bool adminLogin(int cfd)
{
        ssize_t readBytes, writeBytes;            // Number of bytes written to / read from the socket
        char readBuffer[1000], writeBuffer[1000]; // Buffer for reading from / writing to the client
        char tempBuffer[1000];
        //     struct Customer customer;

        int ID;

        memset(readBuffer, 0, sizeof(readBuffer));
        memset(writeBuffer, 0, sizeof(writeBuffer));

        // strcpy(writeBuffer, "Welcome to Admin Menu");        

        // Append the request for LOGIN ID message
        
        strcat(writeBuffer, "\n");

        strcat(writeBuffer, "Enter the username: ");

        writeBytes = write(cfd, writeBuffer, sizeof(writeBuffer));
        if (writeBytes == -1)
        {
                perror("Error writing welcome message to the client!");
                return false;
        }

        readBytes = read(cfd, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
                perror("Error reading username from client!");
                return false;
        }

        // bool userFound = false;

        if (strcmp(readBuffer, "admin1") != 0)
        {
                memset(writeBuffer, 0, sizeof(writeBuffer));
                writeBytes = write(cfd, "Invalid username", sizeof("Invalid username"));
                return false;
        }
        

        memset(writeBuffer, 0, sizeof(writeBuffer));
        writeBytes = write(cfd, "Enter Password: ", sizeof("Enter Password: "));
        if (writeBytes == -1)
        {
                perror("Error writing PASSWORD message to client!");
                return false;
        }

        memset(readBuffer, 0, sizeof(readBuffer));
        readBytes = read(cfd, readBuffer, sizeof(readBuffer));
        if (readBytes == 1)
        {
                perror("Error reading password from the client!");
                return false;
        }

        if (strcmp(readBuffer, "root123") == 0)
        {
                return true;
        }

        memset(writeBuffer, 0, sizeof(writeBuffer));
        writeBytes = write(cfd, "Invalid Password", sizeof("Invalid Password"));

        return false;
}

