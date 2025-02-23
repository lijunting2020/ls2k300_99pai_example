#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int fd;
    char buf1[32]={0};
    char buf2[32]="nihao";
    fd=open("/dev/test", O_RDWR);
    if(fd < 0){
        perror("open error \n");
        return fd;
    }
    printf("open ok\n");
    sleep(10);
    close(fd);
    printf("close ok\n");
    return 0;
}