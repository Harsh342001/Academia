#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include "structs.h"
#include<stdbool.h>

int main(){
    int fd = open("count.txt", O_RDWR|O_CREAT, 0744);

    if(fd==-1){
        perror("error while opening file");
        return 0;
    }

    struct Count count;
    count.courses=0;
    count.faculties=0;
    count.students=0;
    int wb = write(fd, &count, sizeof(struct Count));

    if(wb==-1){
        perror("error while writing");
        return 0;
    }

}