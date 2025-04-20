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
    unsigned int ret = 0;
    fd = open("/dev/s_led", O_RDWR);
    if (fd < 0 ){
        perror("open error\n");
        return fd;
    }
    
    in = atoi(argv[1]);
    if (in == 0) 
    {
        ioctl(fd, OFF_CMD);
    } 
    if (in == 1)
    {
        ioctl(fd, ON_CMD, 1);
    }
    if (in == 2)
    {
        ioctl(fd, ON_CMD, 2);
    }

    if (in == 3)
    {
        write(fd, "hello world ", 13);

    ret = lseek(fd, 0, SEEK_CUR);
    printf("ret:%d\n", ret);

    ret = lseek(fd, 0, SEEK_SET);
    printf("ret:%d\n", ret);

    char readbuf[13] = {0};
    read(fd, readbuf, sizeof(readbuf));
    printf("read:%s\n", readbuf);

    ret = lseek(fd, 0, SEEK_CUR);
    printf("ret:%d\n", ret);

    ret = lseek(fd, -1, SEEK_CUR);
    printf("ret:%d\n", ret);

    write(fd, "Linux", 6);

    ret = lseek(fd, 0, SEEK_CUR);
    printf("ret:%d\n", ret);

    ret = lseek(fd, 0, SEEK_SET);
    printf("ret:%d\n", ret);

    char readbuf1[19] = {0};
    read(fd, readbuf1, sizeof(readbuf1));
    printf("read:%s\n", readbuf1);

    ret = lseek(fd, 0, SEEK_CUR);
    printf("ret:%d\n", ret);
    }

    close(fd); 
    return 0;
}