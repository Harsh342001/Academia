#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>


int main(){
    struct course_count
    {
        int count;
    };
    
    int fd = open("course_count.txt" , O_CREAT | O_RDWR, 0777);

    if(fd <= 0){
        perror("Unable to open the file couser_count file");
        return -1;
    }

    struct course_count cc;

    cc.count = 0;

    int w = write(fd , &cc, sizeof(cc));
    if(w <=0 ){
        perror("unable to store courser count to the file");
    }

    return 0;

}