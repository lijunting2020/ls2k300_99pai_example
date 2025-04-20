#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[ ]){
    int fd;
    char buf [32] = {0};
    fd = open("/dev/s_led", O_RDWR);
    if (fd < 0 ){
        perror("open error\n");
        return fd;
    }
    buf[0] = atoi(argv[1]);
    write(fd, buf, sizeof(buf));
    close(fd); 
    return 0;
}