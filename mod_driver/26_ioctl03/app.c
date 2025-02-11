#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define CMD_TEST1 _IOW('L', 1, int)

struct args
{
    int a;
    int b;
    int c;
};

int main(int argc, char *argv[]){
    int fd;
    fd=open("/dev/test", O_RDWR);
    if(fd < 0){
        perror("open error \n");
        return fd;
    }

    struct args test;
    test.a=1;
    test.b=0;
    test.c=1;
    while(1)
    {
        ioctl(fd, CMD_TEST1, &test);
        sleep(1);
    }

    close(fd);
    return 0;
}