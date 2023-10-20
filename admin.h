#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include <errno.h>
#include<stdlib.h>
// #include "auth.h"

// struct Admin;


void addStudent(int cfd){

        
        char rbuf[1024],wbuf[1024];
        // printf("wbuf: %ld",sizeof(wbuf));
        memset(rbuf, 0, sizeof(rbuf));
        memset(wbuf, 0, sizeof(wbuf));

        struct Student student;

        strcpy(wbuf, "Enter the Name of Student: ");
        int wb = write(cfd, wbuf, strlen(wbuf));
        if(wb==-1){
                perror("Error while writing to socket");
                return;
        }
        memset(wbuf, 0, sizeof(wbuf));

        fflush(stdin);
        fflush(stdout);
        int rb = read(cfd, rbuf, sizeof(rbuf));
        
        rbuf[rb]='\0';

        memset(student.name, 0, 256);

        strcpy(student.name, rbuf);

        memset(rbuf, 0, sizeof(rbuf));

        strcpy(wbuf, "Enter the username of student: ");
        wb = write(cfd, wbuf, strlen(wbuf));
        if(wb==-1){
                perror("Error while writing to socket");
                return;
        }
        memset(wbuf, 0, sizeof(wbuf));

        fflush(stdin);
        fflush(stdout);

        rb = read(cfd, rbuf, sizeof(rbuf));
        
        rbuf[rb]='\0';

        memset(student.username, 0, 50);
        strcpy(student.username, rbuf);

        memset(rbuf, 0, sizeof(rbuf));


        strcpy(wbuf, "Enter the branch of student: ");
        wb = write(cfd, wbuf, strlen(wbuf));
        if(wb==-1){
                perror("Error while writing to socket");
                return;
        }
        memset(wbuf, 0, sizeof(wbuf));

        fflush(stdin);
        fflush(stdout);

        rb = read(cfd, rbuf, sizeof(rbuf));
        
        rbuf[rb]='\0';

        memset(student.branch, 0, 10);
        strcpy(student.branch, rbuf);

        memset(rbuf, 0, sizeof(rbuf));
        
        strcpy(wbuf, "Enter the degree of student: ");
        wb = write(cfd, wbuf, strlen(wbuf));
        if(wb==-1){
                perror("Error while writing to socket");
                return;
        }
        memset(wbuf, 0, sizeof(wbuf));

        fflush(stdin);
        fflush(stdout);

        rb = read(cfd, rbuf, sizeof(rbuf));
        
        rbuf[rb]='\0';

        memset(student.degree, 0, 10);
        strcpy(student.degree, rbuf);

        memset(rbuf, 0, sizeof(rbuf));


        memset(student.password, 0, 50);
        strcpy(student.password, "student");

        memset(student.courses, -1, 6*sizeof(int));

        student.isActivated=true;
        student.isBlocked=false;

        int fd_count = open("count.txt", O_RDWR);

        if(fd_count==-1){
                perror("error opening count file");
                return;
        }

        struct Count count;
        rb = read(fd_count, &count, sizeof(struct Count));
        
        student.rollno = count.students;

        count.students++;

        int offset_count = lseek(fd_count, -sizeof(struct Count), SEEK_CUR);
        if(offset_count==-1){
                perror("error while seeking to required record");
                return;
        }

        wb = write(fd_count, &count, sizeof(struct Count));

        close(fd_count);

        int fd_student = open("students.txt", O_WRONLY|O_CREAT, 0744);

        if(fd_student==-1){
                perror("error while opening student file");
                return;
        }

        int offset_stu = lseek(fd_student, 0, SEEK_END);

        if(offset_stu==-1){
                perror("error while seeking to end of the student file");
                return;
        }

        fflush(stdin);
        fflush(stdout);
        wb = write(fd_student, &student, sizeof(struct Student));

        if(wb==-1)
        {
                perror("Error while writing faculty record");
        }

        close(fd_student);


}


bool viewStudentDetails(int cfd){
    char rbuf[1024],wbuf[1024];
    memset(rbuf, 0, sizeof(rbuf));
    memset(wbuf, 0, sizeof(wbuf));

    strcpy(wbuf, "Enter the roll number of student whose details you're searching for: ");

    int wb = write(cfd, wbuf, strlen(wbuf));
    if(wb==-1){
        perror("Error while writing to socket fd");
        return false;
    }

    int rb = read(cfd, rbuf, sizeof(rbuf));
    rbuf[rb]='\0';

    int roll = atoi(rbuf);

    memset(rbuf, 0, sizeof(rbuf));
    memset(wbuf, 0, sizeof(wbuf));

    int fd = open("students.txt", O_RDONLY);
    if(fd==-1){
        perror("Error while opening students file");
        return false;
    }

    struct flock lock = {F_RDLCK, SEEK_SET, 0, 0, getpid()};

    int fs = fcntl(fd, F_SETLKW, &lock);
    if(fs==-1){
        perror("Error while attaining read lock on students file");
        return false;
    }

    struct Student cur;

    bool flag=0;

    while(read(fd, &cur, sizeof(cur))>0){
        if(cur.rollno==roll){
            flag=1;
            break;
        }
    }

    if(!flag){
        // not found
        strcpy(wbuf, "Student with this roll number not found!");
        wb = write(cfd, wbuf, strlen(wbuf));
        if(wb==-1){
            perror("Error while writing to socket");
        }
        return false;
    }

    strcpy(wbuf, "\n**************Required Student Details**************\n");

    char temp[1024];
    sprintf(temp, "\nStudent Name: %s\nRoll Number: %d\nUsername: %s\nBranch: %s\nDegree: %s\n",cur.name,cur.rollno,cur.username,cur.branch,cur.degree);
    if(cur.isActivated)strcat(temp,"Activated: Yes\n");
    else strcat(temp, "Activated: No\n");

    if(cur.isBlocked)strcat(temp, "Blocked: Yes\n");
    else strcat(temp, "Blocked: No\n");

    strcat(wbuf, temp);

    wb = write(cfd, wbuf, strlen(wbuf));
    if(wb==-1){
        perror("Error while writing to socket");
        return false;
    }
    
    lock.l_type = F_UNLCK;
    fs = fcntl(fd, F_SETLKW, &lock);
    if(fs == -1){
        perror("Error while unlocking students file from read lock");
        return false;
    }

    return true;

}



void addFaculty(int cfd){

        char rbuf[1024],wbuf[1024];
        memset(rbuf, 0, sizeof(rbuf));
        memset(wbuf, 0, sizeof(wbuf));

        struct Faculty faculty;

        strcpy(wbuf, "Enter the Name of Faculty: ");
        int wb = write(cfd, wbuf, strlen(wbuf));
        if(wb==-1){
                perror("Error while writing to socket");
                return;
        }
        memset(wbuf, 0, sizeof(wbuf));

        int rb = read(cfd, rbuf, sizeof(rbuf));
        
        rbuf[rb]='\0';
        memset(faculty.name, 0, 256);
        strcpy(faculty.name, rbuf);

        memset(rbuf, 0, sizeof(rbuf));
        // printf("rbuf size: %ld\n",sizeof(rbuf));
        strcpy(wbuf, "Enter the username of faculty: ");
        wb = write(cfd, wbuf, strlen(wbuf));
        if(wb==-1){
                perror("Error while writing to socket");
                return;
        }
        memset(wbuf, 0, sizeof(wbuf));

        rb = read(cfd, rbuf, sizeof(rbuf));
        
        rbuf[rb]='\0';
        memset(faculty.username, 0, 50);
        strcpy(faculty.username, rbuf);

        memset(rbuf, 0, sizeof(rbuf));



        strcpy(wbuf, "Enter the branch of faculty: ");
        wb = write(cfd, wbuf, strlen(wbuf));
        if(wb==-1){
                perror("Error while writing to socket");
                return;
        }
        memset(wbuf, 0, sizeof(wbuf));

        rb = read(cfd, rbuf, sizeof(rbuf));
        
        rbuf[rb]='\0';
        memset(faculty.branch, 0, 10);
        strcpy(faculty.branch, rbuf);

        memset(rbuf, 0, sizeof(rbuf));



        memset(faculty.password, 0, 50);
        strcpy(faculty.password, "faculty");

        memset(faculty.courses, -1, 10*sizeof(int));

        int fd_count = open("count.txt", O_RDWR);

        if(fd_count==-1){
                perror("error opening count file");
                return;
        }    

        struct Count count;
        rb = read(fd_count, &count, sizeof(struct Count));
        
        faculty.faculty_id = count.faculties;

        count.faculties++;

        int offset_count = lseek(fd_count, -sizeof(struct Count), SEEK_CUR);
        if(offset_count==-1){
                perror("error while seeking to required record");
                return;
        }

        wb = write(fd_count, &count, sizeof(struct Count));

        close(fd_count);

        int fd_faculty = open("faculty.txt", O_WRONLY|O_CREAT, 0744);

        if(fd_faculty==-1){
                perror("error while opening faculty file");
                return;
        }

        int offset_fac = lseek(fd_faculty, 0, SEEK_END);

        if(offset_fac==-1){
                perror("error while seeking to end of the file");
                return;
        }

        wb = write(fd_faculty, &faculty, sizeof(struct Faculty));

        if(wb==-1)
        {
                perror("Error while writing faculty record");
        }

        close(fd_faculty);
}


bool viewFacultyDetails(int cfd){
    char rbuf[1024],wbuf[1024];
    memset(rbuf, 0, sizeof(rbuf));
    memset(wbuf, 0, sizeof(wbuf));

    strcpy(wbuf, "Enter the faculty Id of faculty whose details you're searching for: ");

    int wb = write(cfd, wbuf, strlen(wbuf));
    if(wb==-1){
        perror("Error while writing to socket fd");
        return false;
    }

    int rb = read(cfd, rbuf, sizeof(rbuf));
    rbuf[rb]='\0';

    int fid = atoi(rbuf);

    memset(rbuf, 0, sizeof(rbuf));
    memset(wbuf, 0, sizeof(wbuf));

    int fd = open("faculty.txt", O_RDONLY);
    if(fd==-1){
        perror("Error while opening faculty file");
        return false;
    }

    struct flock lock = {F_RDLCK, SEEK_SET, 0, 0, getpid()};

    int fs = fcntl(fd, F_SETLKW, &lock);
    if(fs==-1){
        perror("Error while attaining read lock on faculty file");
        return false;
    }

    struct Faculty cur;

    bool flag=0;

    while(read(fd, &cur, sizeof(cur))>0){
        if(cur.faculty_id==fid){
            flag=1;
            break;
        }
    }

    if(!flag){
        // not found
        strcpy(wbuf, "Faculty with this Id not found!");
        wb = write(cfd, wbuf, strlen(wbuf));
        if(wb==-1){
            perror("Error while writing to socket");
        }
        return false;
    }

    strcpy(wbuf, "\n**************Required Faculty Details**************\n");

    char temp[1024];
    sprintf(temp, "\nFaculty Name: %s\nFaculty Id: %d\nUsername: %s\nBranch: %s\n",cur.name,cur.faculty_id,cur.username,cur.branch);
    
    strcat(wbuf, temp);

    wb = write(cfd, wbuf, strlen(wbuf));
    if(wb==-1){
        perror("Error while writing to socket");
        return false;
    }
    
    lock.l_type = F_UNLCK;
    fs = fcntl(fd, F_SETLKW, &lock);
    if(fs == -1){
        perror("Error while unlocking students file from read lock");
        return false;
    }

    return true;

}


bool blockStudent(int cfd){
        char rbuf[1024],wbuf[1024];
        memset(rbuf, 0, sizeof(rbuf));
        memset(wbuf, 0, sizeof(wbuf));

        strcpy(wbuf, "Enter the id of the student you want to block: ");
        int wb = write(cfd, wbuf, strlen(wbuf));
        if(wb==-1){
                perror("error while writing to client");
                return false;
        }

        int rb = read(cfd, rbuf, sizeof(rbuf));
        
        if(rb==-1){
                perror("error while reading from socket");
                return false;
        }
        int roll = atoi(rbuf);

        int fd = open("students.txt", O_RDWR);
        if(fd==-1){
                perror("error while opening students file");
                close(fd);
                return false;
        }

        int offset = lseek(fd, roll*sizeof(struct Student), SEEK_SET);

        if(offset==-1){
                return false;
        }


        struct Student student;

        struct flock lock = {F_WRLCK, SEEK_CUR, 0, sizeof(struct Student), getpid()};
        int fs = fcntl(fd, F_SETLKW, &lock);

        if(fs==-1){
                perror("error while attaining write lock on student record");
                close(fd);
                return false;
        }

        rb = read(fd, &student, sizeof(struct Student));

        student.isBlocked = true;
        student.isActivated = false;

        offset = lseek(fd, roll*sizeof(struct Student), SEEK_SET);

        wb = write(fd, &student, sizeof(struct Student));

        lock.l_type = F_UNLCK;
        fs = fcntl(fd, F_SETLKW, &lock);
        if(fs==-1){
                perror("error while removing write lock from student record");
                close(fd);
                return false;
        }

        close(fd);

        return true;

}





bool activateStudent(int cfd){
        char rbuf[1024],wbuf[1024];
        memset(rbuf, 0, sizeof(rbuf));
        memset(wbuf, 0, sizeof(wbuf));

        strcpy(wbuf, "Enter the id of the student you want to activate: ");
        int wb = write(cfd, wbuf, strlen(wbuf));
        if(wb==-1){
                perror("error while writing to client");
                return false;
        }

        int rb = read(cfd, rbuf, sizeof(rbuf));
        
        if(rb==-1){
                perror("error while reading from socket");
                return false;
        }
        rbuf[rb]='\0';

        int roll = atoi(rbuf);

        int fd = open("students.txt", O_RDWR);
        if(fd==-1){
                perror("error while opening students file");
                close(fd);
                return false;
        }

        int offset = lseek(fd, roll*sizeof(struct Student), SEEK_SET);

        if(offset==-1){
                return false;
        }


        struct Student student;

        struct flock lock = {F_WRLCK, SEEK_CUR, 0, sizeof(struct Student), getpid()};
        int fs = fcntl(fd, F_SETLKW, &lock);

        if(fs==-1){
                perror("error while attaining write lock on student record");
                close(fd);
                return false;
        }

        rb = read(fd, &student, sizeof(struct Student));

        student.isBlocked = false;
        student.isActivated = true;

        offset = lseek(fd, roll*sizeof(struct Student), SEEK_SET);

        wb = write(fd, &student, sizeof(struct Student));

        lock.l_type = F_UNLCK;
        fs = fcntl(fd, F_SETLKW, &lock);
        if(fs==-1){
                perror("error while removing write lock from student record");
                close(fd);
                return false;
        }

        close(fd);

        return true;

}




bool modifyStudentDetails(int cfd){
        char rbuf[1024],wbuf[1024];
        memset(rbuf, 0, sizeof(rbuf));
        memset(wbuf, 0, sizeof(wbuf));


        strcpy(wbuf, "Enter the roll number of the student whose details you want to edit: ");

        int wb = write(cfd, wbuf, strlen(wbuf));

        

        int rb = read(cfd, rbuf, sizeof(rbuf));

        rbuf[rb]='\0';

        int roll = atoi(rbuf);

        memset(wbuf, 0, sizeof(wbuf));
        memset(rbuf, 0, sizeof(rbuf));

        struct Student st;

        int fd = open("students.txt", O_RDWR);

        if(fd==-1){
                perror("error while opening students file");
                return false;
        }

        while(1){
                struct flock lock = {F_WRLCK, SEEK_CUR, 0, sizeof(struct Student), getpid()};
                int fs = fcntl(fd, F_SETLKW, &lock);
                if(fs==-1){
                        perror("Error while attaining write lock on student record");
                        close(fd);
                        return false;
                }

                rb = read(fd, &st, sizeof(struct Student));
                if(rb==-1){
                        perror("error while reading student record");
                        close(fd);
                        return false;
                }
                
                if(st.rollno == roll){
                        strcpy(wbuf, "What do you want to edit?\n\n1. Name\n2. Branch\n3. Degree\n 4. Username\n\n");
                        int wb = write(cfd, wbuf, strlen(wbuf));

                        int rb = read(cfd, rbuf, sizeof(rbuf));
                        rbuf[rb]='\0';

                        int choice = atoi(rbuf);
                        
                        memset(wbuf, 0, sizeof(wbuf));
                        memset(rbuf, 0, sizeof(rbuf));

                        switch(choice){
                                case 1:
                                {
                                        memset(wbuf, 0, sizeof(wbuf));
                                        memset(rbuf, 0, sizeof(rbuf));
                                        strcpy(wbuf, "Enter the new Name: ");
                                        int wb = write(cfd, wbuf, strlen(wbuf));

                                        int rb = read(cfd, rbuf, sizeof(rbuf));
                                        rbuf[rb]='\0';

                                        memset(st.name, 0, 256);

                                        strcpy(st.name, rbuf);
                                        
                                        break;


                                }
                                case 2:
                                {
                                        memset(wbuf, 0, sizeof(wbuf));
                                        memset(rbuf, 0, sizeof(rbuf));

                                        strcpy(wbuf, "Enter the new branch: ");
                                        int wb = write(cfd, wbuf, strlen(wbuf));

                                        int rb = read(cfd, rbuf, sizeof(rbuf));
                                        rbuf[rb]='\0';

                                        memset(st.branch, 0, 10);

                                        strcpy(st.branch, rbuf);
                                        
                                        break;


                                }
                                
                                case 3:
                                {
                                        memset(wbuf, 0, sizeof(wbuf));
                                        memset(rbuf, 0, sizeof(rbuf));
                                        strcpy(wbuf, "Enter the new degree: ");
                                        int wb = write(cfd, wbuf, strlen(wbuf));

                                        int rb = read(cfd, rbuf, sizeof(rbuf));
                                        rbuf[rb]='\0';

                                        memset(st.degree, 0, 10);

                                        strcpy(st.degree, rbuf);
                                        
                                        break;


                                }
                                case 4:
                                {
                                        memset(wbuf, 0, sizeof(wbuf));
                                        memset(rbuf, 0, sizeof(rbuf));
                                        strcpy(wbuf, "Enter the new username: ");
                                        int wb = write(cfd, wbuf, strlen(wbuf));

                                        int rb = read(cfd, rbuf, sizeof(rbuf));
                                        rbuf[rb]='\0';

                                        memset(st.username, 0, 50);

                                        strcpy(st.username, rbuf);
                                        
                                        break;


                                }

                                default:
                                {
                                        memset(wbuf, 0, sizeof(wbuf));
                                        strcpy(wbuf, "Invalid Choice!\n");
                                        write(cfd, wbuf, strlen(wbuf));
                                        return false;
                                }

                        }

                        int offset = lseek(fd, -sizeof(struct Student), SEEK_CUR);
                        if(offset==-1){
                                perror("Error while seeking to start of the record");
                                return false;
                        }

                        wb = write(fd, &st, sizeof(struct Student));

                        if(wb==-1){
                                perror("error while writing student record");
                                return false;
                        }

                        // close(fd);

                        offset = lseek(fd, -sizeof(struct Student), SEEK_CUR);
                        if(offset==-1){
                                perror("Error while seeking to start of the record");
                                return false;
                        }

                        lock.l_type = F_UNLCK;

                        fs = fcntl(fd, F_SETLKW, &lock);

                        if(fs==-1){
                                perror("error while removing write lock from the record");
                        }

                        close(fd);

                        return true;


                }

                lock.l_type = F_UNLCK;

                fs = fcntl(fd, F_SETLKW, &lock);

                if(fs==-1){
                        perror("error while removing write lock from the record");
                }

        }
 
        memset(wbuf, 0, sizeof(wbuf));
        strcpy(wbuf, "\n\nStudent with this id not found!\n\n");
        
        wb = write(cfd, wbuf, strlen(wbuf));

        return false;

}





bool modifyFacultyDetails(int cfd){
        char rbuf[1024],wbuf[1024];
        memset(rbuf, 0, sizeof(rbuf));
        memset(wbuf, 0, sizeof(wbuf));


        strcpy(wbuf, "Enter the id of the faculty whose details you want to edit: ");

        int wb = write(cfd, wbuf, strlen(wbuf));

        

        int rb = read(cfd, rbuf, sizeof(rbuf));

        rbuf[rb]='\0';

        int id = atoi(rbuf);

        memset(wbuf, 0, sizeof(wbuf));
        memset(rbuf, 0, sizeof(rbuf));

        struct Faculty fac;

        int fd = open("faculty.txt", O_RDWR);

        if(fd==-1){
                perror("error while opening faculty file");
                return false;
        }

        while(1){
                struct flock lock = {F_WRLCK, SEEK_CUR, 0, sizeof(struct Faculty), getpid()};
                int fs = fcntl(fd, F_SETLKW, &lock);
                if(fs==-1){
                        perror("Error while attaining write lock on faculty record");
                        close(fd);
                        return false;
                }

                rb = read(fd, &fac, sizeof(struct Faculty));
                if(rb==-1){
                        perror("error while reading faculty record");
                        close(fd);
                        return false;
                }
                
                if(fac.faculty_id == id){
                        strcpy(wbuf, "What do you want to edit?\n\n1. Name\n2. Branch\n3. Username\n\n");
                        int wb = write(cfd, wbuf, strlen(wbuf));

                        int rb = read(cfd, rbuf, sizeof(rbuf));
                        rbuf[rb]='\0';

                        int choice = atoi(rbuf);
                        
                        memset(wbuf, 0, sizeof(wbuf));
                        memset(rbuf, 0, sizeof(rbuf));

                        switch(choice){
                                case 1:
                                {
                                        memset(wbuf, 0, sizeof(wbuf));
                                        memset(rbuf, 0, sizeof(rbuf));
                                        strcpy(wbuf, "Enter the new Name: ");
                                        int wb = write(cfd, wbuf, strlen(wbuf));

                                        int rb = read(cfd, rbuf, sizeof(rbuf));
                                        rbuf[rb]='\0';

                                        memset(fac.name, 0, 256);

                                        strcpy(fac.name, rbuf);
                                        
                                        break;


                                }
                                case 2:
                                {
                                        memset(wbuf, 0, sizeof(wbuf));
                                        memset(rbuf, 0, sizeof(rbuf));

                                        strcpy(wbuf, "Enter the new branch: ");
                                        int wb = write(cfd, wbuf, strlen(wbuf));

                                        int rb = read(cfd, rbuf, sizeof(rbuf));
                                        rbuf[rb]='\0';

                                        memset(fac.branch, 0, 10);

                                        strcpy(fac.branch, rbuf);
                                        
                                        break;


                                }
                                
                                case 3:
                                {
                                        memset(wbuf, 0, sizeof(wbuf));
                                        memset(rbuf, 0, sizeof(rbuf));
                                        strcpy(wbuf, "Enter the new username: ");
                                        int wb = write(cfd, wbuf, strlen(wbuf));

                                        int rb = read(cfd, rbuf, sizeof(rbuf));
                                        rbuf[rb]='\0';

                                        memset(fac.username, 0, 50);

                                        strcpy(fac.username, rbuf);
                                        
                                        break;


                                }

                                default:
                                {
                                        memset(wbuf, 0, sizeof(wbuf));
                                        strcpy(wbuf, "Invalid Choice!\n");
                                        write(cfd, wbuf, strlen(wbuf));
                                        return false;
                                }

                        }

                        int offset = lseek(fd, -sizeof(struct Faculty), SEEK_CUR);
                        if(offset==-1){
                                perror("Error while seeking to start of the record");
                                return false;
                        }

                        wb = write(fd, &fac, sizeof(struct Faculty));

                        if(wb==-1){
                                perror("error while writing student record");
                                return false;
                        }

                        // close(fd);

                        offset = lseek(fd, -sizeof(struct Faculty), SEEK_CUR);
                        if(offset==-1){
                                perror("Error while seeking to start of the record");
                                return false;
                        }

                        lock.l_type = F_UNLCK;

                        fs = fcntl(fd, F_SETLKW, &lock);

                        if(fs==-1){
                                perror("error while removing write lock from the record");
                        }

                        close(fd);

                        return true;


                }

                lock.l_type = F_UNLCK;

                fs = fcntl(fd, F_SETLKW, &lock);

                if(fs==-1){
                        perror("error while removing write lock from the record");
                }

        }
 
        memset(wbuf, 0, sizeof(wbuf));
        strcpy(wbuf, "\n\nFaculty with this id not found!\n\n");
        
        wb = write(cfd, wbuf, strlen(wbuf));

        return false;

}







void adminMenu(int cfd){
	bool login = adminLogin(cfd);
    if(login){
        // char buf[100] = "Logged in as Admin!";
		// int wb = write(cfd, buf, sizeof(buf));
        // char rbuf[1];
        // int rb = read(cfd, rbuf, 1);

	    // display admin menu
	// if(adminLogin(cfd)){
        while(1){
            char menu[4096] = "\n....................................Welcome to Admin Menu....................................\n"
                    "1. Add Student\n"
                    "2. View Student Details\n"
                    "3. Add Faculty\n"
                    "4. View Faculty Details\n"
                    "5. Activate Student\n"
                    "6. Block Student\n"
                    "7. Modify Student Details\n"
                    "8. Modify Faculty Details\n"
                    "9. Logout and Exit\n\n"
                    "Enter your choice: \r\n";

            int wb = write(cfd, menu, strlen(menu));

            if(wb==-1){
                    perror("Error while displaying menu: ");
                    return;
            }
            
            char chbf[1024];   
            
            int rb = read(cfd, chbf, sizeof(chbf));
            if(rb==-1){
                    perror("Error while reading choice: ");
                    return;
            }


            int choice = atoi(chbf);

            printf("choice: %d\n",choice);
            switch(choice){
                case 1:
                {
                    addStudent(cfd);
                    break;
                }
                case 2:
                {

                    viewStudentDetails(cfd);
                    // if(ret)continue;

                    printf("outside\n");
                    // sleep(1);
                    // char rbf[1024];
                    // int rb = read(cfd, rbf, sizeof(rbf)); 
                    break;
                }
                case 3:
                {
                    addFaculty(cfd);
                        break;
                }
                case 4:
                {
                    viewFacultyDetails(cfd);    
                    break;
                }
                case 5:
                {

                        break;
                }
                case 6:
                {
                        bool isBlocked = blockStudent(cfd);
                        char bf[100];
                        if(!isBlocked){
                                strcpy(bf, "Student couldn't be blocked!\n");
                                wb = write(cfd, bf, strlen(bf));
                                if(wb==-1){
                                        perror("error while writing to socket");
                                }
                        }
                        else{
                               strcpy(bf, "Student blocked Successfully!\n");
                               wb = write(cfd, bf, strlen(bf));
                                if(wb==-1){
                                        perror("error while writing to socket");
                                } 
                        }

                        break;
                }
                case 7:
                {
                        bool modified = modifyStudentDetails(cfd);
                        if(modified){
                                char bf[100];
                                strcpy(bf, "Student Details modified Successfully!\n");
                                wb = write(cfd, bf, strlen(bf));
                                if(wb==-1){
                                        perror("error while writing to socket");
                                } 
                        }
                        
                        break;
                }
                case 8:
                {
                        bool modified = modifyFacultyDetails(cfd);
                        if(modified){
                                char bf[100];
                                strcpy(bf, "Faculty Details modified Successfully!\n");
                                wb = write(cfd, bf, strlen(bf));
                                if(wb==-1){
                                        perror("error while writing to socket");
                                } 
                        }
                        break;
                }
                case 9:
                {
                        return;
                }
                default:
                {
                        // printf("Enter a valid choice!\n");
                        break;
                }
            }

            continue;
            
        }
    }



}


