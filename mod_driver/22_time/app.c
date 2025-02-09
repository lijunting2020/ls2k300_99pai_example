#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int fd;
    unsigned int counter = 0;
    fd=open("/dev/test", O_RDWR);
    if(fd < 0){
        perror("open error \n");
        return fd;
    }
    while (1) {
        read(fd, &counter, sizeof(counter));
        printf("counter is %d\n", counter);
        sleep(1);
    }
    close(fd);
    return 0;
}