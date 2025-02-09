#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int fd;
    unsigned int ret = 0;
    fd=open("/dev/test", O_RDWR);
    if(fd < 0){
        perror("open error \n");
        return fd;
    }

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

    close(fd);
    return 0;
}