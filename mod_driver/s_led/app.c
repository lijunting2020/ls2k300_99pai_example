#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#define OFF_CMD _IO('C', 0)          //关灯 0.
#define ON_CMD _IOW('C', 1, int)     //开灯 1.常亮  2.闪

int main(int argc, char *argv[ ]){
    int fd;
    int in = -1;
    fd = open("/dev/s_led", O_RDWR);
    if (fd < 0 ){
        perror("open error\n");
        return fd;
    }
    
    in = atoi(argv[1]);
    if (in == 0) 
    {
        ioctl(fd, OFF_CMD);
    } else {
        ioctl(fd, ON_CMD, in);
    }

    close(fd); 
    return 0;
}