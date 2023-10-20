#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
// #include "auth.h"

struct Faculty f;

void addCourse(int cfd, int fid){

        int fd = open("courses.txt", O_WRONLY|O_CREAT, 0744);


        struct Course cur;

        if(fd==-1){
                perror("Error while opening course file: ");
                return;
        }
        
        char bf[1024];
        strcpy(bf, "Before CS");
        write(cfd, bf, strlen(bf));

        
        int fd1 = open("count.txt", O_RDWR, 0744);
        if(fd1==-1){
                perror("Error while opening count file: ");
                return;
        }


        char rbuf[1024],wbuf[1024]="Enter name of the course: ";

        int wb = write(cfd, wbuf, strlen(wbuf));
        if(wb==-1){
                perror("Error while asking for name: ");
                return;
        }

        int rb = read(cfd, rbuf, sizeof(rbuf));

        if(rb==-1){
                perror("Error while reading name: ");
                return;
        }
        rbuf[rb] = '\0';
        memset(cur.name, 0, 256);
        strcpy(cur.name, rbuf);

        strcpy(wbuf, "Enter the total intake of the course: ");

        wb = write(cfd, wbuf, strlen(wbuf));
        if(wb==-1){
                perror("Error while asking for intake: ");
                return;
        }

        memset(rbuf, 0, sizeof(rbuf));

        rb = read(cfd, rbuf, sizeof(rbuf));
        rbuf[rb]='\0';

        if(rb==-1){
                perror("Error while reading intake: ");
                return;
        }
        
        int intake = atoi(rbuf);
        cur.intake = intake;

        memset(rbuf, 0, sizeof(rbuf));
        memset(wbuf, 0, sizeof(wbuf));


        strcpy(wbuf, "Enter the branch of the course: ");

        wb = write(cfd, wbuf, strlen(wbuf));
        if(wb==-1){
                perror("Error while asking for branch: ");
                return;
        }

        memset(rbuf, 0, sizeof(rbuf));

        rb = read(cfd, rbuf, sizeof(rbuf));
        rbuf[rb]='\0';

        if(rb==-1){
                perror("Error while reading branch: ");
                return;
        }

        memset(cur.branch, 0, 10);

        strcpy(cur.branch, rbuf);




        cur.current = 0;
        cur.faculty_id = fid;
        cur.isActive = true;
        struct flock lock2 = {F_WRLCK, SEEK_SET, 0, 0, getpid()};

        int fs1 = fcntl(fd1, F_SETLKW, &lock2);
   
        if(fs1==-1){
                perror("error while locking the count file");
        }

        struct Count count;

        rb = read(fd1, &count, sizeof(struct Count));
        
        cur.code = count.courses;

        count.courses++;

        int countoffset = lseek(fd1, 0, SEEK_SET);

        if(countoffset==-1){
                perror("error while seeking to start of count file");
                return;
        }

        wb = write(fd1, &count, sizeof(struct Count));

        if(wb == -1){
                perror("error while writing to count file");
                return;
        }

        lock2.l_type = F_UNLCK;
        fcntl(fd1, F_SETLKW, &lock2);
        
        close(fd1);

        // add the course
        struct flock lock1 = {F_WRLCK, SEEK_SET, 0, 0, getpid()};

        int fs = fcntl(fd, F_SETLKW, &lock1);
        if(fs==-1){
                perror("error while locking the courses file");
        }
        
        int l = lseek(fd, 0, SEEK_END);

        if(l==-1){
                perror("Error while moving file pointer\n");
                return;
        }

        wb = write(fd, &cur, sizeof(struct Course));
        if(wb==-1){
                perror("Error while writing into file ");
                return;
        }

        lock1.l_type = F_UNLCK;
        fcntl(fd, F_SETLKW, &lock1);

        close(fd);

        int fd2 = open("faculty.txt", O_RDWR, 0744);

        struct Faculty faculty;

        int offset = lseek(fd2, fid*sizeof(struct Faculty), SEEK_SET);

        if(offset==-1){
                perror("error seeking to the required record");
                return;
        }

        struct flock lock3 = {F_WRLCK, SEEK_SET, fid, fid*sizeof(struct Faculty), getpid()};
        int st = fcntl(fd2, F_SETLKW, &lock3);

        if(st==-1){
                perror("error attaining write lock on the record");
                return;
        }

        rb = read(fd2, &faculty, sizeof(struct Faculty));

        if(rb==-1){
                perror("error while reading record from the file");
                return;
        }
        

        
        for(int i=0;i<4;i++){
                if(faculty.courses[i]==-1){
                        faculty.courses[i]=cur.code;
                        break;
                }
        }

        offset = lseek(fd2, -sizeof(struct Faculty), SEEK_CUR);

        wb = write(fd2, &faculty, sizeof(struct Faculty));
        if(wb==-1){
                perror("error while writing back faculty record");
        }


        lock3.l_type = F_UNLCK;
        fcntl(fd2, F_SETLKW, &lock3);

        strcpy(wbuf, "Course added successfully\n");

        wb = write(cfd, wbuf, strlen(wbuf));
        close(fd2);
        return;

}




void viewCourses(int cfd, int fid){
        char rbuf[1024],wbuf[4096];
        memset(rbuf, 0, sizeof(rbuf));
        memset(wbuf, 0, sizeof(wbuf));

        int fd = open("faculty.txt", O_RDONLY, 0744);
        if(fd==-1){
                perror("error opening faculty file: ");
                return;
        }
        struct Faculty faculty;
        
        // strcpy(wbuf,"hello\n");
        // write(cfd, wbuf, strlen(wbuf));

        strcpy(wbuf, "\n****************Offered Course Details****************\n");

        while(read(fd, &faculty, sizeof(struct Faculty))>0){
                memset(rbuf, 0, sizeof(rbuf));
                memset(wbuf, 0, sizeof(wbuf));
                if(faculty.faculty_id==fid){
                        struct flock lock = {F_RDLCK, SEEK_SET, fid, fid*sizeof(struct Faculty), getpid()};
                        int st = fcntl(fd, F_SETLKW, &lock);
                        if(st==-1){
                                perror("error locking the file in read mode:");
                                return;
                        }
                        
                        // printf("%s\n",tmpbuf);
                        strcpy(wbuf, "**************Courses Offered**************\n");

                        for(int i=0;i<4;i++){
                                char temp[1024];
                                if(faculty.courses[i]!=-1){
                                        struct Course c = getCourseDetails(faculty.courses[i]);
                                        // printf("cname: %s\n",c.name);
                                        if(c.isActive){
                                                sprintf(temp, "\nCourse Name: %s\nCourse Code: %d\nCourse Branch: %s\nFaculty Id: %d\nStudents Enrolled: %d\nIntake: %d\n", c.name,c.code,c.branch,c.faculty_id,c.current,c.intake);
                                                if(c.isActive){
                                                        strcat(temp, "Active: Yes\n");
                                                }
                                                else strcat(temp, "Active: No\n");
                                                
                                                strcat(wbuf, temp);
                                        }
                                }
                        }

                        int wb = write(cfd, wbuf, strlen(wbuf));
                        if(wb==-1){
                                perror("error while writing to socket");
                                return;
                        }

                        lock.l_type = F_UNLCK;
                        st = fcntl(fd, F_SETLK, &lock);

                        if(st==-1){
                                perror("Error while removing read lock from record");
                        }

                        // printf("wbuf: %s\n",wbuf);
                        
                        return;
                }

        }

        memset(rbuf, 0, sizeof(rbuf));
        memset(wbuf, 0, sizeof(wbuf));

        strcpy(wbuf, "faculty with corresponding id does not exist");

        int wb = write(cfd, wbuf, strlen(wbuf));

}


bool removeCourse(int cfd){
        char rbuf[1024],wbuf[4096];
        
        memset(rbuf, 0, sizeof(rbuf));
        memset(wbuf, 0, sizeof(wbuf));

        strcpy(wbuf, "Enter the id of the course you want to remove: ");
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

        int id = atoi(rbuf);

        int fd = open("courses.txt", O_RDWR);
        if(fd==-1){
                perror("error while opening courses file");
                close(fd);
                return false;
        }

        int offset = lseek(fd, id*sizeof(struct Course), SEEK_SET);

        if(offset==-1){
                return false;
        }


        struct Course course;

        struct flock lock = {F_WRLCK, SEEK_CUR, 0, sizeof(struct Course), getpid()};
        int fs = fcntl(fd, F_SETLKW, &lock);

        if(fs==-1){
                perror("error while attaining write lock on course record");
                close(fd);
                return false;
        }

        rb = read(fd, &course, sizeof(struct Course));

        course.isActive = false;

        offset = lseek(fd, id*sizeof(struct Course), SEEK_SET);

        wb = write(fd, &course, sizeof(struct Course));

        lock.l_type = F_UNLCK;
        fs = fcntl(fd, F_SETLKW, &lock);
        if(fs==-1){
                perror("error while removing write lock from course record");
                close(fd);
                return false;
        }

        close(fd);

        return true;

}




bool changePasswordFac(int cfd){
        char rbuf[1024],wbuf[4096];
        
        memset(rbuf, 0, sizeof(rbuf));
        memset(wbuf, 0, sizeof(wbuf));

        strcpy(wbuf, "Enter new password: ");
        int wb = write(cfd, wbuf, strlen(wbuf));
        if(wb==-1){
                perror("error while writing to client");
                return false;
        }

        int rb = read(cfd, rbuf, sizeof(rbuf));
        if(rb==-1){
                perror("error while reading from client");
                return false;
        }

        rbuf[rb]='\0';

        int fd = open("faculty.txt", O_RDWR);
        if(fd==-1){
                perror("Error while opening faculty file");
                // close(fd);
                return false;
        }

        struct Faculty faculty;

        int offset = lseek(fd, f.faculty_id*sizeof(struct Faculty), SEEK_SET);
        if(offset==-1){
                perror("error while seeking to the required record");
                close(fd);
                return false;
        }

        rb = read(fd, &faculty, sizeof(struct Faculty));

        if(rb==-1){
                perror("error while reading the faculty record");
                close(fd);
                return false;
        }

        memset(faculty.password, 0, 50);
        strcpy(faculty.password, rbuf);

        offset = lseek(fd, f.faculty_id*sizeof(struct Faculty), SEEK_SET);
        if(offset==-1){
                perror("error while seeking to start of record");
                close(fd);
                return false;
        }

        wb = write(fd, &faculty, sizeof(struct Faculty));

        if(wb==-1){
                perror("Error while writing the modified record to file");
                close(fd);
                return false;
        }

        memset(wbuf, 0, sizeof(wbuf));
        strcpy(wbuf, "Password changed successfully!\n");
        wb = write(cfd, wbuf, strlen(wbuf));
        if(wb==-1){
                perror("Error while writing to client");
                close(fd);
                return false;
        }

        close(fd);
        return true;
        
}



void facultyMenu(int cfd){
	bool login = facultyLogin(cfd, &f);
	if(!login){
		// send message to client about invalid login
		char buf[100] = "Invalid username or password\n";
		int wb = write(cfd, buf, strlen(buf));
		return;
	}
        char buf[100] = "Successfully logged in!\n";
        int wb = write(cfd, buf, strlen(buf));
	// display faculty menu
	
        while(1){

                char menu[4096] = "....................................Welcome to Faculty Menu....................................\n"
                                "1. View Offering Courses\n"
                                "2. Add New Course\n"
                                "3. Remove Course from Catalog\n"
                                "4. Update Course Details\n"
                                "5. Change Password\n"
                                "6. Logout and Exit\n\n"
                                "Enter your choice: ";

                wb = write(cfd, menu, strlen(menu));

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

                switch(choice){
                        case 1:
                        {
                                // printf("id: %d\n",f.faculty_id);
                                viewCourses(cfd, f.faculty_id);
                                break;
                        }
                        case 2:
                        {

                                addCourse(cfd, f.faculty_id);
                                // char code[50];
                                // printf("Enter the course code: ");
                                // scanf(" %[^\n]s",code);
                                // enroll(s->rollno, code);
                                break;
                        }
                        case 3:
                        {
                                bool removed = removeCourse(cfd);
                                 
                                char bf[100];
                                if(!removed){
                                        strcpy(bf, "Course couldn't be removed\n");
                                        wb = write(cfd, bf, strlen(bf));
                                        if(wb==-1){
                                                perror("error while writing to socket");
                                        }
                                }
                                else{
                                strcpy(bf, "Course removed Successfully!\n");
                                wb = write(cfd, bf, strlen(bf));
                                        if(wb==-1){
                                                perror("error while writing to socket");
                                        } 
                                }

                                break;
                        }
                        case 4:
                        {
                                break;
                        }
                        case 5:
                        {
                                bool changed = changePasswordFac(cfd);
                                if(!changed){
                                        char buf[100]="Unable to change password!";
                                        wb = write(cfd, buf, strlen(buf));
                                }
                                break;
                        }
                        case 6:
                        {
                                return;
                        }
                        default:
                        {
                                // printf("Enter a valid choice!\n");
                                break;
                        }
                }
        }



}


