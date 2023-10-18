#include <stdio.h>
#include <string.h>
#include "structures.h"
#include <fcntl.h>
#include <unistd.h>     




void addFaculty(int client_socket){

    int send_res;
    int recv_res;
    char buf[1024];
    char response[1024];
    struct Faculty faculty;

    int fd = open("faculty_data.txt", O_RDWR| O_CREAT | O_APPEND,0777);
    if(fd == -1){
        perror("Error opening file ");
        return;
    }

    int fd_c = open("faculty_count.txt", O_RDWR);
    if(fd_c == -1){
        perror("Error opening file ");
        return;
    }

    struct faculty_count fc;
    
    int r = read(fd_c , &fc, sizeof(fc));

    if(r <= 0){
        perror("unable to read faculty count");
        return ;
    }
    
    printf("read count of faculty is : %d \n", fc.count);
    fc.count = fc.count + 1;
    faculty.faculty_id = fc.count;
    printf("new count to be written is : %d \n" , fc.count);
    printf("%ld" , lseek(fd_c,0, SEEK_SET));
    int w = write(fd_c , &fc, sizeof(fc));

    if(w <= 0){
        perror("unable to write faculty count");
        return ;
    }

    strcpy(buf , "Enter name : ");
    send_res = send(client_socket , buf , strlen(buf),0);
    if(send_res <= 0){
        perror("error sending student parameters");
        return;
    }

    recv_res = recv(client_socket , response , sizeof(response),0);
    if(recv_res <= 0){
        perror("Error recieving student response");
        return;
    }
    strcpy(faculty.name ,response);
    // sd.name  = response;

    strcpy(buf , "Enter username : ");
    send_res = send(client_socket , buf , strlen(buf),0);
    if(send_res <= 0){
        perror("error sending student parameters");
        return;
    }

    recv_res = recv(client_socket , response , sizeof(response),0);
    if(recv_res <= 0){
        perror("Error recieving student response");
        return;
    }
   
    strcpy(faculty.username,response);
    strcpy(faculty.password,"ps123");
    memset(faculty.courses, 0, sizeof(faculty.courses));
    int sd_write = write(fd , &faculty , sizeof(faculty));
    if(sd_write == -1){
        perror("error occured adding student data to the file");
        return;
    }
    printf("Data written to file \n");



    // printf("sd -> %s" , sd.name);

}


void viewFaculty(int clien_socket){
    struct Faculty faculty;  
    char buf[1024]; 
    // sprintf(buf, "\n Faculty_name : %s \n Faculty_id: %d ",fd)

    int fd = open("faculty_data.txt", O_RDONLY);
    if(fd == -1){
        perror("Error opening file ");
        return;
    }

    while(read(fd , &faculty , sizeof(faculty)) > 0 ){
        printf("faculty_id : %d \n",faculty.faculty_id );
        printf("faculty_name. %s \n" , faculty.name);
        // printf("")

    }

}

void viewStudents(int clien_socket){
    struct Student sd;  
   

    int fd = open("student_data.txt", O_RDONLY);
    if(fd == -1){
        perror("Error opening file ");
        return;
    }

    char rbuf[1024],wbuf[10000];
        memset(rbuf, 0, sizeof(rbuf));
        memset(wbuf, 0, sizeof(wbuf));

        struct Course course;

        while(read(fd, &sd, sizeof(sd))>0){
                
                char temp[1000];
                sprintf(temp, "Student name : %s  \nUsername : %s \n\n\t***\n\n", sd.name,sd.username);
                strcat(wbuf, temp);
    
        }

        int writeBytes = write(clien_socket, wbuf, strlen(wbuf));
        if (writeBytes == -1)
        {
                perror("Error writing course record info to client!");
                return;
        }

    // while(read(fd , &sd , sizeof(sd)) > 0 ){
    //     printf("name : %s \n",sd.name );
    //     printf("Roll no. %s \n" , sd.rollno);
    //     // printf("")

    // }

}

void addStudent(int client_socket){

    int send_res;
    int recv_res;
    char buf[1024];
    char response[1024];
    struct Student sd;

    int fd = open("student_data.txt", O_WRONLY | O_APPEND);
    if(fd == -1){
        perror("Error opening file ");
        return;
    }


    strcpy(buf , "Enter name : ");
    send_res = send(client_socket , buf , strlen(buf),0);
    if(send_res <= 0){
        perror("error sending student parameters");
        return;
    }

    recv_res = recv(client_socket , response , sizeof(response),0);
    if(recv_res <= 0){
        perror("Error recieving student response");
        return;
    }
    strcpy(sd.name ,response);
    // sd.name  = response;

    strcpy(buf , "Enter Roll no : ");
    send_res = send(client_socket , buf , strlen(buf),0);
    if(send_res <= 0){
        perror("error sending student parameters");
        return;
    }

    recv_res = recv(client_socket , response , sizeof(response),0);
    if(recv_res <= 0){
        perror("Error recieving student response");
        return;
    }
    strcpy(sd.rollno,response);
    strcpy(sd.username,response);
    strcpy(sd.password,"ps123");
    memset(sd.courses, 0, sizeof(sd.courses));
    int sd_write = write(fd , &sd , sizeof(sd));
    if(sd_write == -1){
        perror("error occured adding student data to the file");
        return;
    }




    // printf("sd -> %s" , sd.name);

}

void editStudent(int client_socket){
    printf("editing student \n");
    char edit_buf[1024] = "Enter the student roll number you wanna edit \n";

    int send_res = send(client_socket , edit_buf , strlen(edit_buf),0);
    if(send_res <= 0){
        perror("error sending student parameters");
        return;
    }

    memset(edit_buf, 0, sizeof(edit_buf));
    
    int recv_res = recv(client_socket , edit_buf , sizeof(edit_buf),0);
    if(recv_res <= 0){
        perror("Error recieving student response");
        return;
    }
    edit_buf[recv_res]='\0';



    int fd = open("student_data.txt", O_RDWR);
    if(fd == -1){
        perror("Error opening file ");
        return;
    }
    printf("client looking for : %s" , edit_buf);
    // char buf_student[1024];
    
    struct Student sd;  
    // int count = 0;
    int found=0;
    while(read(fd , &sd , sizeof(sd)) > 0 ){
        printf("Looking for roll number : %s", sd.rollno);
        if(!strcmp(edit_buf , sd.rollno)){
            found = 1;
            printf("got the student you were looking for \n");
             break;
        }

    }


    // printf("");
    if(!found){
        printf("student with the entered roll number not found \n");
        return;
    }

    printf("Found student : %s \n", sd.rollno);
    char buf[1024] = "Enter the field that you wanna edit : \n"
    "1. name\n"
    "2. rollno\n"
    "3. username \n"
    "4. password \n";

     send_res = send(client_socket , buf , strlen(buf),0);
    if(send_res <= 0){
        perror("error sending student parameters");
        return;
    }

    recv_res = recv(client_socket , buf , sizeof(buf),0);
    if(recv_res <= 0){
        perror("Error recieving student response");
        return;
    }

    char edit_val[1024];

     send_res = send(client_socket , "Enter the value you wanna update it to" , strlen("Enter the value you wanna update it to"),0);
    if(send_res <= 0){
        perror("error sending student parameters");
        return;
    }


     recv_res = recv(client_socket , edit_val , sizeof(edit_val),0);
    if(recv_res <= 0){
        perror("Error recieving student response");
        return;
    }

   switch(atoi(buf)){
    case 1: 
        strcpy(sd.name , edit_val);
        break;
    case 2: 
        strcpy(sd.rollno , edit_val);
        break;
    case 3: 
        strcpy(sd.username , edit_val);
        break;
    case 4: 
        strcpy(sd.password , edit_val);     
        break; 
   }

    printf("%ld" , lseek(fd, -sizeof(sd), SEEK_CUR)); 

    int w  = write(fd  , &sd , sizeof(sd));
    if(w == -1){
        perror("updated write of student failed");
    }
}


void send_menu(int client_socket){
        
while(1){

    char admin_menu[4096] = "1 . Add Student\n"
    "2 . Add Faculty\n"
    "3 . View Students\n"
    "4 . View Facultiies \n"
    "5 . Activate/Deactivate student\n"
    "6 . Update student/faculty\n";
    "7 . Exit\n";


    int send_value = send(client_socket , admin_menu , strlen(admin_menu),0);
    if(send_value <= 0){
        printf("enter valid value");
    }
    else{
        char choice_buffer[1024];
        int choice_bytes = recv(client_socket, choice_buffer, sizeof(choice_buffer),0);
         if(choice_bytes <= 0){
            printf("enter valid choice \n");
         }
         else{
            if(atoi(choice_buffer) == 7){
                break;
            }
            printf("choice entered by client: %s\n", choice_buffer);
            switch (atoi(choice_buffer))
            {
            case 1:
                addStudent(client_socket);
                // printf("add student called");
                break;
            case 2:
                addFaculty(client_socket);
                // printf("add student called");
                break;    
            case 3:
                viewStudents(client_socket);
                // printf("add student called");
                break;
            case 4:
                viewFaculty(client_socket);
                // printf("add student called");
                break;
            case 6:
                
                editStudent(client_socket);
                // printf("add student called");
                break;    
            default:
                break;
            }
            }
        

        

    }
    
    
}
    

}
