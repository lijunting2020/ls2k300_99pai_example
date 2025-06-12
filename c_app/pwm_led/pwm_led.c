#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h> 
#include <signal.h>  
#include <time.h> 

#define SYSFS_PWM_DIR "/sys/class/pwm"
#define MAX_BUF 11

int my_period = 1000000;//1000000000;
float rate;
int my_duty;

int pwm_export(unsigned int pwm)
{
    int fd;
    switch (pwm) {
        case 1:{
            fd = open(SYSFS_PWM_DIR "/pwmchip0/export", O_WRONLY);
            if(fd < 0){
                printf("\nFaild export pwm_0\n");
                return -1;
            }
            write(fd, "0", 2);
            close(fd);
            return 0;
            break;
        }
        case 2:{
            fd = open(SYSFS_PWM_DIR "/pwmchip1/export", O_WRONLY);
            if(fd < 0){
                printf("\nFaild export pwm_1\n");
                return -1;
            }
            write(fd, "0", 2);
            close(fd);
            return 0;
            break;
        }
        case 3:{
            fd = open(SYSFS_PWM_DIR "/pwmchip2/export", O_WRONLY);
            if(fd < 0){
                printf("\nFaild export pwm_2\n");
                return -1;
            }
            write(fd, "0", 2);
            close(fd);
            return 0;
            break;
        }
    }
    return  0;
}

int pwm_unexport(unsigned int pwm)
{
    int fd;
    switch (pwm) {
        case 1:{
            fd = open(SYSFS_PWM_DIR "/pwmchip0/unexport", O_WRONLY);
            if(fd < 0){
                printf("\nFaild unexport pwm_0\n");
                return -1;
            }
            write(fd, "0", 2);
            close(fd);
            return 0;
            break;
        }
        case 2:{
            fd = open(SYSFS_PWM_DIR "/pwmchip1/unexport", O_WRONLY);
            if(fd < 0){
                printf("\nFaild unexport pwm_b\n");
                return -1;
            }
            write(fd, "0", 2);
            close(fd);
            return 0;
            break;
        }
        case 3:{
            fd = open(SYSFS_PWM_DIR "/pwmchip2/unexport", O_WRONLY);
            if(fd < 0){
                printf("\nFaild unexport pwm_b\n");
                return -1;
            }
            write(fd, "0", 2);
            close(fd);
            return 0;
            break;
        }
    }
    return  0;
}

int pwm_disable(unsigned int pwm)
{
    int fd;
    switch (pwm) {
        case 1:{
            fd = open(SYSFS_PWM_DIR "/pwmchip0/pwm0/enable", O_WRONLY);
            if(fd < 0){
                printf("\nFaild enable pwm_b\n");
                return -1;
            }
            write(fd, "0", 2);
            close(fd);
            return 0;
            break;
        }
        case 2:{
            fd = open(SYSFS_PWM_DIR "/pwmchip1/pwm0/enable", O_WRONLY);
            if(fd < 0){
                printf("\nFaild enable pwm_b\n");
                return -1;
            }
            write(fd, "0", 2);
            close(fd);
            return 0;
            break;
        }
        case 3:{
            fd = open(SYSFS_PWM_DIR "/pwmchip2/pwm0/enable", O_WRONLY);
            if(fd < 0){
                printf("\nFaild enable pwm_b\n");
                return -1;
            }
            write(fd, "0", 2);
            close(fd);
            return 0;
            break;
        }
    }
    return 0;
}

int pwm_config(unsigned int pwm, unsigned int period, unsigned int duty_cycle)
{
    int fd, len_p, len_d;
    char buf_p[MAX_BUF];
    char buf_d[MAX_BUF];
    len_p = snprintf(buf_p, sizeof(buf_p), "%d", period);
    len_d = snprintf(buf_d, sizeof(buf_d), "%d", duty_cycle);
    switch (pwm) {
        case 1:{
            //fd = open(SYSFS_PWM_DIR "/pwmchip0/pwm0/period", O_WRONLY);
            //if(fd < 0){
            //    printf("\nFaild period pwm_1 period:%u", period);
            //    return -1;
            //}
            //write(fd, buf_p, len_p);

            fd = open(SYSFS_PWM_DIR "/pwmchip0/pwm0/duty_cycle", O_WRONLY);
            if(fd < 0){
                printf("\nFaild duty_cycle pwm_1 duty_cycle:%u", duty_cycle);
                return -1;
            }
            write(fd, buf_d, len_d);
            close(fd);
            return 0;
            break;
        }
        case 2:{
            fd = open(SYSFS_PWM_DIR "/pwmchip1/pwm0/period", O_WRONLY);
            if(fd < 0){
                printf("\nFaild period pwm_2\n");
                return -1;
            }
            write(fd, buf_p, len_p);

            fd = open(SYSFS_PWM_DIR "/pwmchip1/pwm0/duty_cycle", O_WRONLY);
            if(fd < 0){
                printf("\nFaild duty_cycle pwm_2\n");
                return -1;
            }
            write(fd, buf_d, len_d);
            close(fd);
            return 0;
            break;
        }
        case 3:{
            fd = open(SYSFS_PWM_DIR "/pwmchip2/pwm0/period", O_WRONLY);
            if(fd < 0){
                printf("\nFaild period pwm_3\n");
                return -1;
            }
            write(fd, buf_p, len_p);

            fd = open(SYSFS_PWM_DIR "/pwmchip2/pwm0/duty_cycle", O_WRONLY);
            if(fd < 0){
                printf("\nFaild duty_cycle pwm_3\n");
                return -1;
            }
            write(fd, buf_d, len_d);
            close(fd);
            return 0;
            break;
        }
    }
    return 0;
}

int pwm_polarity(unsigned int pwm, int polarity)
{
    int fd;
    switch (pwm) {
        case 1:{
            fd = open(SYSFS_PWM_DIR "/pwmchip0/pwm0/polarity", O_WRONLY);
            if(fd < 0){
                printf("\nFaild polarity pwm_0\n");
                return -1;
            }
            if(polarity == 1){
                write(fd, "normal", 6);
            }else{
                write(fd, "inversed", 8);
            }
            close(fd);
            return 0;
            break;
        }
        case 2:{
            fd = open(SYSFS_PWM_DIR "/pwmchip1/pwm0/polarity", O_WRONLY);
            if(fd < 0){
                printf("\nFaild polarity pwm_1\n");
                return -1;
            }
            if(polarity == 1){
                write(fd, "normal", 6);
            }else{
                write(fd, "inversed", 8);
            }
            close(fd);
            return 0;
            break;
        }
        case 3:{
            fd = open(SYSFS_PWM_DIR "/pwmchip2/pwm0/polarity", O_WRONLY);
            if(fd < 0){
                printf("\nFaild polarity pwm_2\n");
                return -1;
            }
            if(polarity == 1){
                write(fd, "normal", 6);
            }else{
                write(fd, "inversed", 8);
            }
            close(fd);
            return 0;
            break;
        }
    }
    return 0;
}

int pwm_enable(unsigned int pwm)
{
    int fd;
    switch (pwm) {
        case 1:{
            fd = open(SYSFS_PWM_DIR "/pwmchip0/pwm0/enable", O_WRONLY);
            if(fd < 0){
                printf("\nFaild enable pwm_b\n");
                return -1;
            }
            write(fd, "1", 2);
            close(fd);
            return 0;
            break;
        }
        case 2:{
            fd = open(SYSFS_PWM_DIR "/pwmchip1/pwm0/enable", O_WRONLY);
            if(fd < 0){
                printf("\nFaild enable pwm_b\n");
                return -1;
            }
            write(fd, "1", 2);
            close(fd);
            return 0;
            break;
        }
        case 3:{
            fd = open(SYSFS_PWM_DIR "/pwmchip2/pwm0/enable", O_WRONLY);
            if(fd < 0){
                printf("\nFaild enable pwm_b\n");
                return -1;
            }
            write(fd, "1", 2);
            close(fd);
            return 0;
            break;
        }
    }
    return 0;
}

void at_signal (int sig)
{
    for(int i=1; i<4; i++){
        if(pwm_unexport(i) < 0){
            perror("pwm unexport err");
            printf("pwm %d\n", i);
        }
    }
}

void at_exit(void) {
    for(int i=1; i<4; i++){
        if(pwm_unexport(i) < 0){
            perror("pwm unexport err");
            printf("pwm %d\n", i);
        }
    }
}

int main()
{
    signal(SIGINT, at_signal);
    atexit(at_exit);

    //printf("start pwm\n");

    while(1){
        scanf("%f", &rate);
        if(rate <= 0){
            printf("exit\n");
            break;
        }
    my_duty = my_period * rate;
    for(int i=1; i<4; i++){
        //printf("pwm %d export start...\n", i);
        if(pwm_export(i) < 0){
            printf("pwm %d export error...\n", i);
            return -1;
        }
        
        //printf("pwm %d disable start...\n", i);
        if(pwm_disable(i) < 0){
            printf("pwm %d disable error...\n", i);
            return -1;
        }
    }

    if(pwm_config(1, my_period, my_duty) < 0){
        printf("pwm 1 configure error...\n");
        return -1;
    }
    if(pwm_polarity(1, 0) < 0){
        printf("pwm 1 polarity error...\n");
        return -1;
    }
    if(pwm_enable(1) < 0){
        printf("pwm 1 enable error...\n");
        return -1;
    }

    if(pwm_config(2, my_period, my_period) < 0){
        printf("pwm 2 configure error...\n");
        return -1;
    }
    if(pwm_polarity(2, 1) < 0){
        printf("pwm 2 polarity error...\n");
        return -1;
    }
    if(pwm_enable(2) < 0){
        printf("pwm 2 enable error...\n");
        return -1;
    }

    if(pwm_config(3, my_period, my_period) < 0){
        printf("pwm 3 configure error...\n");
        return -1;
    }
    if(pwm_polarity(3, 1) < 0){
        printf("pwm 3 polarity error...\n");
        return -1;
    }
    if(pwm_enable(3) < 0){
        printf("pwm 3 enable error...\n");
        return -1;
    }
//while (1) {
    for(int i=1;i<= 10; i++)
    {
        if(i>10){
            i=1;
        }

        double duty_rate = i/10.00;
        //printf("pwm 1 configure duty_rate:%f", duty_rate);
        unsigned int duty_int = my_period*duty_rate;
        //printf("\npwm 1 configure duty_cycle:%u\n", duty_int);

        time_t now = time(NULL); // 获取当前时间
        struct tm *local_time = localtime(&now); // 转换为本地时间
        printf("开始时间:%02d:%02d:%02d  period-%d毫秒 duty cycle - %2.2f%%\n",local_time->tm_hour,local_time->tm_min,local_time->tm_sec, my_period/1000000, ((double)duty_int/my_period*100));

        if(pwm_config(1, my_period, duty_int) < 0){
            printf("pwm 1 configure error...\n");
            return -1;
        }
        usleep((rate * 1000 * 1000)/10);
    }
    //time_t now2 = time(NULL); // 获取当前时间
    //struct tm *local_time2 = localtime(&now2); // 转换为本地时间
    //printf("结束时间:%02d:%02d:%02d",local_time2->tm_hour,local_time2->tm_min,local_time2->tm_sec);
    //usleep(5000000);
//}

    /*
    for(int i=1; i<4; i++){
        printf("pwm %d export start...\n", i);
        if(pwm_export(i) < 0){
            printf("pwm %d export error...\n", i);
            return -1;
        }
        
        printf("pwm %d disable start...\n", i);
        if(pwm_disable(i) < 0){
            printf("pwm %d disable error...\n", i);
            return -1;
        }

        printf("pwm %d configure start...\n", i);
        if(pwm_config(i, my_period, my_duty) < 0){
            printf("pwm %d configure error...\n", i);
            return -1;
        }

        printf("pwm %d polarity start...\n", i);
        if(pwm_polarity(i, 0) < 0){
            printf("pwm %d polarity error...\n", i);
            return -1;
        }

        printf("pwm %d enable start...\n", i);
        if(pwm_enable(i) < 0){
            printf("pwm %d enable error...\n", i);
            return -1;
        }
    }
    */
    //printf("pwm_a successfully enabled with period - %dms, duty cycle - %2.1f%%\n", my_period/1000000, rate * 100);
}
    return 0;
}