#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define CMD_TEST0 _IO('L', 0)
#define CMD_TEST1 _IO('L', 1)
#define CMD_TEST2 _IO('A', 0)
#define CMD_TEST3 _IOW('A', 1, int)
#define CMD_TEST4 _IOR('A', 2, int)
#define CMD_TEST5 _IOWR('A', 3, int)

int main(int argc, char *argv[]){
    printf("CMD_TEST3 type is %ld\n", _IOC_TYPE(CMD_TEST3));
    printf("CMD_TEST3 dir is %ld\n", _IOC_DIR(CMD_TEST3));
    printf("CMD_TEST3 nr is %ld\n", _IOC_NR(CMD_TEST3));
    printf("CMD_TEST3 size is %ld\n", _IOC_SIZE(CMD_TEST3));
    return 0;
}