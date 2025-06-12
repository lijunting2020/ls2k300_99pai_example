#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

//记录开启状态：true.开启、false.关闭
int is_run = 1;

void closed_led ()
{
    int fd;
    //将三个灯置为灭
    fd = open("/sys/class/gpio/gpio45/value", O_WRONLY);
    write(fd, "0", 1);
    close(fd);

    fd = open("/sys/class/gpio/gpio42/value", O_WRONLY);
    write(fd, "0", 1);
    close(fd);

    fd = open("/sys/class/gpio/gpio44/value", O_WRONLY);
    write(fd, "0", 1);
    close(fd);
}

int gpio_init ()
{
    // 导出GPIO引脚
    int fd;
    fd = open("/sys/class/gpio/export", O_WRONLY);
    write(fd, "42", 2);
    write(fd, "44", 2);
    write(fd, "45", 2);

    //按键GPIO使用
    write(fd, "43", 2);
    close(fd);

    // 设置GPIO引脚的方向,out表示输出、"in"表示输入
    fd = open("/sys/class/gpio/gpio42/direction", O_WRONLY);
    write(fd, "out", 3); 
    close(fd);

    fd = open("/sys/class/gpio/gpio44/direction", O_WRONLY);
    write(fd, "out", 3);
    close(fd);

    fd = open("/sys/class/gpio/gpio45/direction", O_WRONLY);
    write(fd, "out", 3);
    close(fd);

    //按键GPIO使用
    fd = open("/sys/class/gpio/gpio43/direction", O_WRONLY);
    write(fd, "in", 3);
    close(fd);

    //将三个灯置为灭
    closed_led ();
}

char read_key ()
{
    char value;
    int fd;
    //按键GPIO使用,读取GPIO的值
    fd = open("/sys/class/gpio/gpio43/value", O_RDONLY);
    read(fd, &value, 1);
    close(fd);
    return value;
}

//按键状态检测
void* checkKey(void* args)
{
    while(1)
    {
        char key_value = read_key();
        if(key_value == '1')
        {
            is_run = !is_run;
            if(is_run)
            {
                printf("\nLED START...\n");;
            }
            else
            {
                printf("\nLED STOP...\n");
            }
            usleep(2000000);//2秒
        }
        else
        {
            usleep(200000);//200毫秒
        }
    }

}

int start_green ()
{
    int fd;
     // 设置GPIO引脚的值
    fd = open("/sys/class/gpio/gpio45/value", O_WRONLY);
    write(fd, "1", 1);
    usleep(10000000);  //等待10秒，usleep单位是微秒

    write(fd, "0", 1);
    close(fd);
}

int start_yellow ()
{
    int fd;
     // 设置GPIO引脚的值
    fd = open("/sys/class/gpio/gpio44/value", O_WRONLY);
    write(fd, "1", 1);   //亮1秒
    usleep(500000);

    write(fd, "0", 1);   //灭0.5秒
    usleep(500000);

    write(fd, "1", 1);   //亮0.5秒
    usleep(500000);

    write(fd, "0", 1);   //灭0.5秒
    usleep(500000);

    write(fd, "1", 1);   //亮0.5秒
    usleep(500000);

    write(fd, "0", 1);   //灭

    close(fd);
}

int start_red ()
{
    int fd;
     // 设置GPIO引脚的值
    fd = open("/sys/class/gpio/gpio42/value", O_WRONLY);
    write(fd, "1", 1);   //亮7秒
    usleep(7000000);

    write(fd, "0", 1);   //灭0.5秒
    usleep(500000);

    write(fd, "1", 1);   //亮0.5秒
    usleep(500000);

    write(fd, "0", 1);   //灭0.5秒
    usleep(500000);

    write(fd, "1", 1);   //亮0.5秒
    usleep(500000);

    write(fd, "0", 1);   //灭0.5秒
    usleep(500000);

    write(fd, "1", 1);   //亮0.5秒
    usleep(500000);

    write(fd, "0", 1);   //灭

    close(fd);
}

void gpio_uninit (int sig)
{
    int fd;
    //将三个灯置为灭
    fd = open("/sys/class/gpio/gpio45/value", O_WRONLY);
    write(fd, "0", 1);
    close(fd);

    fd = open("/sys/class/gpio/gpio42/value", O_WRONLY);
    write(fd, "0", 1);
    close(fd);

    fd = open("/sys/class/gpio/gpio44/value", O_WRONLY);
    write(fd, "0", 1);
    close(fd);
    // 清除GPIO引脚
    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    write(fd, "42", 2);
    write(fd, "44", 2);
    write(fd, "45", 2);
    //按键GPIO使用
    write(fd, "43", 2);
    close(fd);

    printf("\nstop...\n");
    exit(0);
}

int main(int argc, char **argv) 
{
    //5.释放函数,将已导出的GPIO释放
    //           将红黄绿三个灯的状态置为熄灭
    signal(SIGINT, gpio_uninit);

    printf("\nstart...\n");

    //1.初始化函数，导出GPIO42、GPIO44、GPIO45
    //              设置信号为输出
    //              将红黄绿三个灯的状态置为熄灭
    gpio_init ();

    pthread_t checkKey_t;
    pthread_create(&checkKey_t, NULL, checkKey, NULL);
    
    while(1)
    {
        if(is_run)
        {
            //2.绿灯亮函数,绿灯持续亮10秒，后熄灭
            start_green ();
            //3.黄灯亮函数,黄灯闪烁3次，后熄灭 
            start_yellow ();
             //4.红灯亮函数,红灯持续亮7秒，闪烁3次，后熄灭
             start_red ();
        }
        else
        {
            closed_led ();
            usleep(200000);
        }
    }
    return 0;
}
