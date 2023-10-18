#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>     


void viewOfferedCourses(int client_socket){
   
    struct Course course;  
   

    int fd = open("courses_data.txt", O_RDONLY);
    if(fd == -1){
        perror("Error opening file ");
        return;
    }

    while(read(fd , &course , sizeof(course)) > 0 ){
        printf("name : %d \n",course.code );
        printf("Roll no. %s \n" ,course.name);
        // printf("")

    }

}


void send_student_menu(int client_socket){
    while(1){

    char student_menu[4096] = "1. View Offered Courses\n"
    "2 . Enroll new Course\n"
    "3 . Drop Enrolled Course\n"
    "4. View Enrolled Courses\n"
    "5 . Password Change \n"
    "6 . Exit\n";


    int send_value = send(client_socket ,student_menu , strlen(student_menu),0);
    if(send_value <= 0){
        printf("enter valid value \n");
    }
    else{
        char choice_buffer[1024];
        int choice_bytes = recv(client_socket, choice_buffer, sizeof(choice_buffer),0);
         if(choice_bytes <= 0){
            printf("enter valid choice \n");
         }
         else{
            // if(atoi(choice_buffer) == 7){
            //     break;
            // }
            printf("choice entered by client: %s\n", choice_buffer);
            if(atoi(choice_buffer) == 6){
                break;
                return;
            }
            switch (atoi(choice_buffer))
            {
            case 1:
                printf("view offered courses\n");
                viewOfferedCourses(client_socket);
                break;
            case 2:
                printf("Enroll new Course\n");
                // addCourse(client_socket , faculty_fd);
                break;    
            case 3:
               
                printf("Remove course\n");
                break;
            case 4:
                printf("change password called\n");
                break;
            // case 6:
            //     printf("Exit menu");

            //     break;    
            default:
                break;
            }
            }
        

        

    }
  } 
}

void handle_student_auth(int client_socket){

    char username[1024];
    char password[1024];
    // printf("enter username");
    char buf_un[] = "Enter Username: ";
    send(client_socket , buf_un, strlen(buf_un),0);

    memset(username , 0, sizeof(username));
    int bytes_read = recv(client_socket, username, sizeof(username),0);
    if(bytes_read == -1){
        perror("username read failed in server");
        return ;
    }

    char buf_ps[]= "Enter Password : ";
    send(client_socket ,buf_ps , strlen(buf_ps),0);
    memset(password , 0, sizeof(password));
    int ps_read = recv(client_socket, password, sizeof(password),0);
    if(ps_read == -1){
        perror("read failed in server");
        return;
    }

    printf("Entered username : %s \n" , username);
    printf("Entered password : %s \n" , password);

    int fd = open("student_data.txt", O_RDWR);
    if(fd == -1){
        perror("Error opening file ");
        return;
    }
        // char buf_student[1024];
    
    struct Student student;  
    // int count = 0;
    int found=0;
    while(read(fd , &student , sizeof(student)) > 0 ){
        printf("Looking for username : %s , %s ", student.username , student.password);
        if(!strcmp(username , student.username) && !strcmp(password , student.password)){
            found = 1;
            // faculty = &f;
            
            // strcpy(faculty.username , f.username) ;
            // strcpy(faculty.password , f.password);
            // strcpy(faculty.name , f.name);
            // faculty.faculty_id = f.faculty_id;
            // memset(faculty.courses , 0 , sizeof(faculty.courses));
            // printf("name of faculty %s" ,);
            printf("Login Successful \n");
            
            send_student_menu(client_socket);
             break;
        }

    }
    if(!found){
        printf("Couldn't Login , Please enter valid credentials");
        return;
    }
}