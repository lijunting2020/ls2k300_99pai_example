#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define CMD_TEST0 _IO('L', 0)

int main(int argc, char *argv[]){
    int fd;
    unsigned int ret = 0;
    fd=open("/dev/test", O_RDWR);
    if(fd < 0){
        perror("open error \n");
        return fd;
    }

    while (1) {
        ioctl(fd, CMD_TEST0);
        sleep(1);
    }

    close(fd);
    return 0;
}