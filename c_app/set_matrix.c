#include <fcntl.h>     //O_WRONLY、open
#include <unistd.h>    //write
#include <stdio.h>     //printf
#include <stdlib.h>    //exit
#include <signal.h>    //signal

int gpio_init ()
{
    // 导出GPIO引脚
    int fd;
    fd = open("/sys/class/gpio/export", O_WRONLY);
    write(fd, "42", 2);
    write(fd, "43", 2);
    write(fd, "44", 2);
    write(fd, "45", 2);

    write(fd, "60", 2);
    write(fd, "61", 2);
    write(fd, "62", 2);
    write(fd, "63", 2);
    close(fd);

    // 设置GPIO引脚的方向,out表示输出、"in"表示输入
    fd = open("/sys/class/gpio/gpio42/direction", O_WRONLY);
    write(fd, "out", 3); 
    close(fd);
    fd = open("/sys/class/gpio/gpio43/direction", O_WRONLY);
    write(fd, "out", 3);
    close(fd);
    fd = open("/sys/class/gpio/gpio44/direction", O_WRONLY);
    write(fd, "out", 3);
    close(fd);
    fd = open("/sys/class/gpio/gpio45/direction", O_WRONLY);
    write(fd, "out", 3);
    close(fd);

    fd = open("/sys/class/gpio/gpio60/direction", O_WRONLY);
    write(fd, "in", 3); 
    close(fd);
    fd = open("/sys/class/gpio/gpio61/direction", O_WRONLY);
    write(fd, "in", 3);
    close(fd);
    fd = open("/sys/class/gpio/gpio62/direction", O_WRONLY);
    write(fd, "in", 3);
    close(fd);
    fd = open("/sys/class/gpio/gpio63/direction", O_WRONLY);
    write(fd, "in", 3);
    close(fd);
}

int gpio_uninit ()
{
    int fd;
    // 清除GPIO引脚
    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    write(fd, "42", 2);
    write(fd, "43", 2);
    write(fd, "44", 2);
    write(fd, "45", 2);

    write(fd, "60", 2);
    write(fd, "61", 2);
    write(fd, "62", 2);
    write(fd, "63", 2);
    
    close(fd);

    printf("\nstop...\n");
    exit(0);
}

void at_signal (int sig)
{
    gpio_uninit ();
}

void at_exit(void) {
    gpio_uninit ();
}

int key_scan ()
{
    char value;
    int fd;

    //1.1.第一行的GPIO设置为高电平;
    fd = open("/sys/class/gpio/gpio45/value", O_WRONLY);
    write(fd, "0", 1); 
    close(fd);

    //测试
    fd = open("/sys/class/gpio/gpio45/value", O_RDONLY);
    read(fd, &value, 1);
    close(fd);
    printf("\nshow gpio45 value %d\n", value);

    //1.2.检测4个列信号，是否有高电位，如果有则说明按下键
    //1.2.1检测第一列
    fd = open("/sys/class/gpio/gpio60/value", O_RDONLY);
    read(fd, &value, 1);
    close(fd);
    printf("\nshow gpio60 value %d\n", value);
    // if(value == '1')
    // {
    //     printf("\nEnter Key s1\n");
    //     fd = open("/sys/class/gpio/gpio60/value", O_RDONLY);
    //     write(fd, "0", 1); 
    //     close(fd);
    //     return 0;
    // }

    //1.2.2检测第二列
    fd = open("/sys/class/gpio/gpio63/value", O_RDONLY);
    read(fd, &value, 1);
    close(fd);
    printf("\nshow gpio63 value %d\n", value);
    // if(value == '1')
    // {
    //     printf("\nEnter Key %s\n", mKey[0][1]);;
    // }

    //1.2.3检测第三列
    fd = open("/sys/class/gpio/gpio61/value", O_RDONLY);
    read(fd, &value, 1);
    close(fd);
    printf("\nshow gpio61 value %d\n", value);
    // if(value == '1')
    // {
    //     printf("\nEnter Key %s\n", mKey[0][2]);;
    // }

    //1.2.4检测第四列
    fd = open("/sys/class/gpio/gpio62/value", O_RDONLY);
    read(fd, &value, 1);
    close(fd);
    printf("\nshow gpio62 value %d\n", value);
    // if(value == '1')
    // {
    //     printf("\nEnter Key %s\n", mKey[0][3]);;
    // }
/*
    //1.3.第一行的GPIO设置为低电平;
    // fd = open("/sys/class/gpio/gpio45/value", O_WRONLY);
    // write(fd, "0", 1); 
    // close(fd);

    //2.1.第二行的GPIO设置为高电平;
    fd = open("/sys/class/gpio/gpio44/value", O_WRONLY);
    write(fd, "1", 1); 
    close(fd);

    //2.2.检测4个列信号，是否有高电位，如果有则说明按下键
    //2.2.1检测第一列
    fd = open("/sys/class/gpio/gpio60/value", O_RDONLY);
    read(fd, &value, 1);
    close(fd);
    if(value == '1')
    {
        printf("\nEnter Key %s\n", mKey[1][0]);;
    }

    //2.2.2检测第二列
    fd = open("/sys/class/gpio/gpio63/value", O_RDONLY);
    read(fd, &value, 1);
    close(fd);
    if(value == '1')
    {
        printf("\nEnter Key %s\n", mKey[1][1]);;
    }

    //2.2.3检测第三列
    fd = open("/sys/class/gpio/gpio61/value", O_RDONLY);
    read(fd, &value, 1);
    close(fd);
    if(value == '1')
    {
        printf("\nEnter Key %s\n", mKey[1][2]);;
    }

    //2.2.4检测第四列
    fd = open("/sys/class/gpio/gpio62/value", O_RDONLY);
    read(fd, &value, 1);
    close(fd);
    if(value == '1')
    {
        printf("\nEnter Key %s\n", mKey[1][3]);;
    }

    //2.3.第一行的GPIO设置为低电平;
    fd = open("/sys/class/gpio/gpio44/value", O_WRONLY);
    write(fd, "0", 1); 
    close(fd);

    //3.1.第一行的GPIO设置为高电平;
    fd = open("/sys/class/gpio/gpio42/value", O_WRONLY);
    write(fd, "1", 1); 
    close(fd);

    //3.2.检测4个列信号，是否有高电位，如果有则说明按下键
    //3.2.1检测第一列
    fd = open("/sys/class/gpio/gpio60/value", O_RDONLY);
    read(fd, &value, 1);
    close(fd);
    if(value == '1')
    {
        printf("\nEnter Key %s\n", mKey[2][0]);;
    }

    //3.2.2检测第二列
    fd = open("/sys/class/gpio/gpio63/value", O_RDONLY);
    read(fd, &value, 1);
    close(fd);
    if(value == '1')
    {
        printf("\nEnter Key %s\n", mKey[2][1]);;
    }

    //3.2.3检测第三列
    fd = open("/sys/class/gpio/gpio61/value", O_RDONLY);
    read(fd, &value, 1);
    close(fd);
    if(value == '1')
    {
        printf("\nEnter Key %s\n", mKey[2][2]);;
    }

    //3.2.4检测第四列
    fd = open("/sys/class/gpio/gpio62/value", O_RDONLY);
    read(fd, &value, 1);
    close(fd);
    if(value == '1')
    {
        printf("\nEnter Key %s\n", mKey[2][3]);;
    }

    //3.3.第一行的GPIO设置为低电平;
    fd = open("/sys/class/gpio/gpio42/value", O_WRONLY);
    write(fd, "0", 1); 
    close(fd);

    //4.1.第一行的GPIO设置为高电平;
    fd = open("/sys/class/gpio/gpio43/value", O_WRONLY);
    write(fd, "1", 1); 
    close(fd);

    //4.2.检测4个列信号，是否有高电位，如果有则说明按下键
    //4.2.1检测第一列
    fd = open("/sys/class/gpio/gpio60/value", O_RDONLY);
    read(fd, &value, 1);
    close(fd);
    if(value == '1')
    {
        printf("\nEnter Key %s\n", mKey[3][0]);;
    }

    //4.2.2检测第二列
    fd = open("/sys/class/gpio/gpio63/value", O_RDONLY);
    read(fd, &value, 1);
    close(fd);
    if(value == '1')
    {
        printf("\nEnter Key %s\n", mKey[3][1]);;
    }

    //4.2.3检测第三列
    fd = open("/sys/class/gpio/gpio61/value", O_RDONLY);
    read(fd, &value, 1);
    close(fd);
    if(value == '1')
    {
        printf("\nEnter Key %s\n", mKey[3][2]);;
    }

    //4.2.4检测第四列
    fd = open("/sys/class/gpio/gpio62/value", O_RDONLY);
    read(fd, &value, 1);
    close(fd);
    if(value == '1')
    {
        printf("\nEnter Key %s\n", mKey[3][3]);;
    }

    //4.3.第一行的GPIO设置为低电平;
    fd = open("/sys/class/gpio/gpio43/value", O_WRONLY);
    write(fd, "0", 1); 
    close(fd);
    */
    return 0;
}

int main(int argc, char **argv) 
{
    //5.释放函数,将已导出的GPIO释放
    //           将红黄绿三个灯的状态置为熄灭
    signal(SIGINT, at_signal);
    atexit(at_exit);

    printf("\nstart...\n");

    //1.初始化函数，导出GPIO42、GPIO43、GPIO44、GPIO45
    //              导出GPIO60、GPIO61、GPIO62、GPIO63
    //              4个接口设置为输出，4个接口设置为输入
    gpio_init ();
    key_scan();
    // while(1)
    // {
    //     key_scan();
    //     usleep(200000);
    // }
    return 0;
}