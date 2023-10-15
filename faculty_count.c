#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>


int main(){
    struct faculty_count
    {
        int count;
    };
    
    int fd = open("faculty_count.txt" , O_CREAT | O_RDWR, 0777);

    if(fd <= 0){
        perror("Unable to open the file");
        return -1;
    }

    struct faculty_count fc;

    fc.count = 0;

    int w = write(fd , &fc, sizeof(fc));
    if(w <=0 ){
        perror("unable to store faculty count to the file");
    }

    return 0;

}