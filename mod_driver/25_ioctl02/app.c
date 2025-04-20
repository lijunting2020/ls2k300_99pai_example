#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define CMD_TEST0 _IO('L', 0)
#define CMD_TEST1 _IOW('L', 1, int)
#define CMD_TEST2 _IOR('L', 2, int)

int main(int argc, char *argv[]){
    int fd;
    unsigned int ret = 0;
    fd=open("/dev/test", O_RDWR);
    if(fd < 0){
        perror("open error \n");
        return fd;
    }

    int val;
    for(int i=0; i<10; i++)
    {
        ioctl(fd, CMD_TEST0);
        sleep(1);
        ioctl(fd, CMD_TEST1, 1);
        sleep(1);
        ioctl(fd, CMD_TEST2, &val);
        printf("CMD_TEST2 val is %d\n", val);
        sleep(1);

    }

    close(fd);
    return 0;
}