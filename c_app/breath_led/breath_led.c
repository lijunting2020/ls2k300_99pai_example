#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SYSFS_PWM_DIR "/sys/class/pwm"
#define PWM64 "/pwmchip0"
#define PWM65 "/pwmchip1"
#define PWM66 "/pwmchip2"
#define MAX_BUF 11

int my_period = 1000000; // 1000000000;
// float rate;
int cmd = 0;
int my_duty;

int pwm_export(unsigned int pwm) {
  int fd;
  char *sysFile;

  switch (pwm) {
  case 1:
    sysFile = SYSFS_PWM_DIR PWM64 "/export";
    break;
  case 2:
    sysFile = SYSFS_PWM_DIR PWM65 "/export";
    break;
  case 3:
    sysFile = SYSFS_PWM_DIR PWM66 "/export";
    break;
  }
  fd = open(sysFile, O_WRONLY);
  if (fd < 0) {
    printf("\nFaild export pwm_%c\n", pwm);
    return -1;
  }
  write(fd, "0", 2);
  close(fd);
  return 0;
}

int pwm_unexport(unsigned int pwm) {
  int fd;
  char *sysFile;

  switch (pwm) {
  case 1:
    sysFile = SYSFS_PWM_DIR PWM64 "/unexport";
    break;
  case 2:
    sysFile = SYSFS_PWM_DIR PWM65 "/unexport";
    break;
  case 3:
    sysFile = SYSFS_PWM_DIR PWM66 "/unexport";
    break;
  }
  fd = open(sysFile, O_WRONLY);
  if (fd < 0) {
    printf("\nFaild unexport pwm_%c\n", pwm);
    return -1;
  }
  write(fd, "0", 2);
  close(fd);
  return 0;
}

int pwm_disable(unsigned int pwm) {
  int fd;
  char *sysFile;

  switch (pwm) {
  case 1:
    sysFile = SYSFS_PWM_DIR PWM64 "/pwm0/enable";
    break;
  case 2:
    sysFile = SYSFS_PWM_DIR PWM65 "/pwm0/enable";
    break;
  case 3:
    sysFile = SYSFS_PWM_DIR PWM66 "/pwm0/enable";
    break;
  }
  fd = open(sysFile, O_WRONLY);
  if (fd < 0) {
    printf("\nFaild enable pwm_b\n");
    return -1;
  }
  write(fd, "0", 2);
  close(fd);
  return 0;
}

int pwm_config_period(unsigned int pwm, unsigned int period) {
  int fd, len_p;
  char buf_p[MAX_BUF];
  len_p = snprintf(buf_p, sizeof(buf_p), "%d", period);
  char *sysFile;

  switch (pwm) {
  case 1:
    sysFile = SYSFS_PWM_DIR PWM64 "/pwm0/period";
    break;
  case 2:
    sysFile = SYSFS_PWM_DIR PWM65 "/pwm0/period";
    break;
  case 3:
    sysFile = SYSFS_PWM_DIR PWM66 "/pwm0/period";
    break;
  }

  fd = open(sysFile, O_WRONLY);
  if (fd < 0) {
    printf("\nFaild period pwm_%c period:%u", pwm, period);
    return -1;
  }
  write(fd, buf_p, len_p);
  close(fd);
  return 0;
}

int pwm_config_duty_cycle(unsigned int pwm, unsigned int duty_cycle) {
  int fd, len_d;
  char buf_d[MAX_BUF];
  len_d = snprintf(buf_d, sizeof(buf_d), "%d", duty_cycle);
  char *sysFile;

  switch (pwm) {
  case 1:
    sysFile = SYSFS_PWM_DIR PWM64 "/pwm0/duty_cycle";
    break;
  case 2:
    sysFile = SYSFS_PWM_DIR PWM65 "/pwm0/duty_cycle";
    break;
  case 3:
    sysFile = SYSFS_PWM_DIR PWM66 "/pwm0/duty_cycle";
    break;
  }
  fd = open(sysFile, O_WRONLY);
  if (fd < 0) {
    printf("\nFaild duty_cycle pwm_%c\n", pwm);
    return -1;
  }
  write(fd, buf_d, len_d);
  close(fd);
  return 0;
}

int pwm_polarity(unsigned int pwm, int polarity) {
  int fd;
  char *sysFile;

  switch (pwm) {
  case 1:
    sysFile = SYSFS_PWM_DIR PWM64 "/pwm0/polarity";
    break;
  case 2:
    sysFile = SYSFS_PWM_DIR PWM65 "/pwm0/polarity";
    break;
  case 3:
    sysFile = SYSFS_PWM_DIR PWM66 "/pwm0/polarity";
    break;
  }
  fd = open(sysFile, O_WRONLY);
  if (fd < 0) {
    printf("\nFaild polarity pwm_%c\n", pwm);
    return -1;
  }
  if (polarity == 1) {
    write(fd, "normal", 6);
  } else {
    write(fd, "inversed", 8);
  }
  close(fd);
  return 0;
}

int pwm_enable(unsigned int pwm) {
  int fd;
  char *sysFile;

  switch (pwm) {
  case 1:
    sysFile = SYSFS_PWM_DIR PWM64 "/pwm0/enable";
    break;
  case 2:
    sysFile = SYSFS_PWM_DIR PWM65 "/pwm0/enable";
    break;
  case 3:
    sysFile = SYSFS_PWM_DIR PWM66 "/pwm0/enable";
    break;
  }
  fd = open(sysFile, O_WRONLY);
  if (fd < 0) {
    printf("\nFaild enable pwm_b\n");
    return -1;
  }
  write(fd, "1", 2);
  close(fd);
  return 0;
  return 0;
}

void at_signal(int sig) {
  for (int i = 1; i < 4; i++) {
    if (pwm_unexport(i) < 0) {
      perror("pwm unexport err");
      printf("pwm %d\n", i);
    }
  }
}

void at_exit(void) {
  for (int i = 1; i < 4; i++) {
    if (pwm_unexport(i) < 0) {
      perror("pwm unexport err");
      printf("pwm %d\n", i);
    }
  }
}

int init() {
  // 初始化3种颜色的灯为不发光
  for (int i = 1; i < 4; i++) {
    if (pwm_export(i) < 0) {
      return -1;
    }

    if (pwm_disable(i) < 0) {
      return -1;
    }
    if (pwm_config_period(i, my_period) < 0) {
      return -1;
    }
    if (pwm_config_duty_cycle(i, my_period) < 0) {
      return -1;
    }
    if (pwm_polarity(i, 1) < 0) {
      return -1;
    }
    if (pwm_enable(i) < 0) {
      return -1;
    }
  }
}

int main() {
  init();

  while (1) {
    scanf("%d", &cmd);
    if (cmd == -1) {
      init();
      printf("exit\n");
      break;
    }

    if (cmd == 0) {
      printf(
          "请输入功能代码:\n-1.退出\n0.说明\n1.绿色\n2.蓝色\n3.红色\n4.黄色\n"
          "5.橙色\n6.青色\n7.紫色\n8.全混色\n11.绿呼吸\n12.蓝呼吸\n13."
          "红呼吸\n");
    }

    if (cmd == 1) {
      init();
      if (pwm_disable(1) < 0) {
        return -1;
      }
      if (pwm_polarity(1, 0) < 0) {
        return -1;
      }
      if (pwm_enable(1) < 0) {
        return -1;
      }
    }

    if (cmd == 2) {
      init();
      if (pwm_disable(2) < 0) {
        return -1;
      }
      if (pwm_polarity(2, 0) < 0) {
        return -1;
      }
      if (pwm_enable(2) < 0) {
        return -1;
      }
    }

    if (cmd == 3) {
      init();
      if (pwm_disable(3) < 0) {
        return -1;
      }
      if (pwm_polarity(3, 0) < 0) {
        return -1;
      }
      if (pwm_enable(3) < 0) {
        return -1;
      }
    }
    if (cmd == 4) {
      init();
      if (pwm_disable(1) < 0) {
        return -1;
      }
      if (pwm_disable(3) < 0) {
        return -1;
      }
      if (pwm_polarity(1, 0) < 0) {
        return -1;
      }
      if (pwm_polarity(3, 0) < 0) {
        return -1;
      }
      if (pwm_enable(1) < 0) {
        return -1;
      }
      if (pwm_enable(3) < 0) {
        return -1;
      }
    }
    if (cmd == 5) {
      init();
      if (pwm_disable(1) < 0) {
        return -1;
      }
      if (pwm_disable(3) < 0) {
        return -1;
      }
      if (pwm_polarity(1, 0) < 0) {
        return -1;
      }
      if (pwm_polarity(3, 0) < 0) {
        return -1;
      }
      if (pwm_enable(1) < 0) {
        return -1;
      }
      if (pwm_enable(3) < 0) {
        return -1;
      }
    }
    if (cmd == 6) {
      init();
      if (pwm_disable(1) < 0) {
        return -1;
      }
      if (pwm_disable(2) < 0) {
        return -1;
      }
      if (pwm_polarity(1, 0) < 0) {
        return -1;
      }
      if (pwm_polarity(2, 0) < 0) {
        return -1;
      }
      if (pwm_enable(1) < 0) {
        return -1;
      }
      if (pwm_enable(2) < 0) {
        return -1;
      }
    }
    if (cmd == 7) {
      init();
      if (pwm_disable(2) < 0) {
        return -1;
      }
      if (pwm_disable(3) < 0) {
        return -1;
      }
      if (pwm_polarity(2, 0) < 0) {
        return -1;
      }
      if (pwm_polarity(3, 0) < 0) {
        return -1;
      }
      if (pwm_enable(2) < 0) {
        return -1;
      }
      if (pwm_enable(3) < 0) {
        return -1;
      }
    }
    if (cmd == 8) {
      init();
      if (pwm_disable(1) < 0) {
        return -1;
      }
      if (pwm_disable(2) < 0) {
        return -1;
      }
      if (pwm_disable(3) < 0) {
        return -1;
      }
      if (pwm_polarity(1, 0) < 0) {
        return -1;
      }
      if (pwm_polarity(2, 0) < 0) {
        return -1;
      }
      if (pwm_polarity(3, 0) < 0) {
        return -1;
      }
      if (pwm_enable(1) < 0) {
        return -1;
      }
      if (pwm_enable(2) < 0) {
        return -1;
      }
      if (pwm_enable(3) < 0) {
        return -1;
      }
    }
    if (cmd > 10) {
      init();
      int rgbNum = 1;
      switch (cmd) {
      case 11:
        rgbNum = 1;
        break;
      case 12:
        rgbNum = 2;
        break;
      case 13:
        rgbNum = 3;
        break;
      }
      for (int i = 1; i <= 10; i++) {
        my_duty = my_period * (i * 0.1);

        time_t now = time(NULL);                 // 获取当前时间
        struct tm *local_time = localtime(&now); // 转换为本地时间
        printf("开始时间:%02d:%02d:%02d  period-%d毫秒  my_duty: %d  duty "
               "cycle % 2.2f % %\n",
               local_time->tm_hour, local_time->tm_min, local_time->tm_sec,
               (my_period / 1000000), my_duty,
               ((double)my_duty / my_period * 100));

        if (pwm_disable(rgbNum) < 0) {
          printf("pwm %d disable error...\n", 1);
          return -1;
        }

        if (pwm_config_duty_cycle(rgbNum, my_duty) < 0) {
          printf("pwm 1 config_duty_cycle error...\n");
          return -1;
        }

        if (pwm_polarity(rgbNum, 0) < 0) {
          return -1;
        }

        if (pwm_enable(rgbNum) < 0) {
          return -1;
        }
        usleep(1000 * 1000);
      }
    }
  }
  return 0;
}