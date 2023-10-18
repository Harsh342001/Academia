#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>     

// struct Faculty *faculty;
  
void addCourse(int client_socket , int faculty_fd){
    // printf("faculty_Name : %s", faculty->name);
    int send_res;
    int recv_res;
    char buf[1024];
    char response[1024];
    struct Course c;

    int fd = open("courses_data.txt", O_RDWR| O_CREAT | O_APPEND,0777);
    if(fd == -1){
        perror("Error opening file ");
        return;
    }

    int fd_c = open("course_count.txt", O_RDWR);
    if(fd_c == -1){
        perror("Error opening file ");
        return;
    }

    struct course_count cc;
    
    int r = read(fd_c , &cc, sizeof(cc));

    if(r <= 0){
        perror("unable to read course count");
        return ;
    }

    // printf("faculty_Name : %s", faculty->name);
    
    printf("read count of course is : %d \n", cc.count);
    cc.count = cc.count + 1;
    c.code = cc.count;
    printf("new count to be written is : %d \n" , cc.count);
    printf("%ld \n" , lseek(fd_c,0, SEEK_SET));
    int w = write(fd_c , &cc, sizeof(cc));

    if(w <= 0){
        perror("unable to write course count");
        return ;
    }

    strcpy(buf , "Enter Course Name : ");
    send_res = send(client_socket , buf , strlen(buf),0);
    if(send_res <= 0){
        perror("error sending student parameters");
        return;
    }
    memset(response , 0 , sizeof(response));
    recv_res = recv(client_socket , response , sizeof(response),0);
    if(recv_res <= 0){
        perror("Error recieving student response");
        return;
    }
    strcpy(c.name ,response);
    // sd.name  = response;
    c.enrolled = 0;

    struct Faculty faculty;
    
    printf("lseeek faculty read : %ld ", lseek(faculty_fd, -sizeof(faculty) , SEEK_CUR));

    r = read(faculty_fd , &faculty , sizeof(faculty));
    printf("read faculty name : %s \n" ,  faculty.name);

    for(int i=0; i < 10 ; i++){
        printf("current course array corresponding to faculty is %d \n", faculty.courses[i]);
    }

    for(int i=0; i <10; i++){
        if(faculty.courses[i] ==0){
            faculty.courses[i] = c.code;
             printf("value of courses array for faculty, %d \n", faculty.courses[i]);
            break;
        }
    }
    
   
     printf("lseek : %ld \n" , lseek(faculty_fd, -sizeof(faculty) , SEEK_CUR)) ;
     printf("course id : %d" , c.code);
     printf("course_name %s: " , c.name);
    int sd_write = write(fd , &c , sizeof(c));
    if(sd_write == -1){
        perror("error occured adding course data to the file");
        return;
    }

     sd_write = write(faculty_fd , &faculty , sizeof(faculty));
    if(sd_write == -1){
        perror("error occured adding student data to the file");
        return;
    }
    

    // printf("sd -> %s" , sd.name);

}

void send_faculty_menu(int client_socket ,int faculty_fd){
        
while(1){

    char faculty_menu[4096] = "1. View Offered Courses\n"
    "2 . Add new Course\n"
    "3 . Remove offered Course\n"
    "4 . Password Change \n"
    "5 . Exit\n";


    int send_value = send(client_socket ,faculty_menu , strlen(faculty_menu),0);
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
            if(atoi(choice_buffer) == 5){
                break;
                return;
            }
            switch (atoi(choice_buffer))
            {
            case 1:
                printf("view offered courses\n");
                break;
            case 2:
                printf("add new Course\n");
                addCourse(client_socket , faculty_fd);
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




void handle_faculty_auth(int client_socket){

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

    int ps_read = recv(client_socket, password, sizeof(password),0);
    if(ps_read == -1){
        perror("read failed in server");
        return;
    }

    printf("Entered username : %s \n" , username);
    printf("Entered password : %s \n" , password);

    int fd = open("faculty_data.txt", O_RDWR);
    if(fd == -1){
        perror("Error opening file ");
        return;
    }
        // char buf_student[1024];
    
    struct Faculty f;  
    // int count = 0;
    int found=0;
    while(read(fd , &f , sizeof(f)) > 0 ){
        printf("Looking for username : %s , %s ", f.username , f.password);
        if(!strcmp(username , f.username) && !strcmp(password , f.password)){
            found = 1;
            // faculty = &f;
            
            // strcpy(faculty.username , f.username) ;
            // strcpy(faculty.password , f.password);
            // strcpy(faculty.name , f.name);
            // faculty.faculty_id = f.faculty_id;
            // memset(faculty.courses , 0 , sizeof(faculty.courses));
            // printf("name of faculty %s" ,);
            printf("Login Successful \n");
            
            send_faculty_menu(client_socket ,fd);
             break;
        }

    }
    if(!found){
        printf("Couldn't Login , Please enter valid credentials");
        return;
    }
}