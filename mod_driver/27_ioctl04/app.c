#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define TIME_CLOSE_CMD _IO('L', 0)
#define TIME_OPEN_CMD _IO('L', 1)
#define TIME_SETTIME_CMD _IOW('L', 2, int)

int main(int argc, char *argv[]){
    int fd;
    unsigned int counter = 0;
    fd=open("/dev/test", O_RDWR);
    if(fd < 0){
        perror("open error \n");
        return fd;
    }

    ioctl(fd, TIME_SETTIME_CMD, 1000);
    ioctl(fd, TIME_OPEN_CMD);
    sleep(3);
    ioctl(fd, TIME_SETTIME_CMD, 3000);
    sleep(7);
    ioctl(fd, TIME_CLOSE_CMD);
    close(fd);
    return 0;
}