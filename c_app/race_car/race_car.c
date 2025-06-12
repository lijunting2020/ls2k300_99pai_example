#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define SYSFS_PWM_DIR "/sys/class/pwm"
#define PWM64 "/pwmchip0"
#define PWM65 "/pwmchip1"
#define PWM66 "/pwmchip2"
#define PWM67 "/pwmchip3"

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define GPIO42 "/gpio42"
#define GPIO43 "/gpio43"
#define GPIO44 "/gpio44"
#define GPIO45 "/gpio45"

#define GPIO60 "/gpio60"
#define GPIO61 "/gpio61"
#define GPIO62 "/gpio62"
#define GPIO63 "/gpio63"

#define MAX_BUF 11

int my_period = 1000000; // 1000000000;
// float rate;
int cmd = 0;
int my_duty;

int gpio_export(char *gpio) {
  int fd;
  fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
  if (fd < 0) {
    printf("gpio_export error %s", gpio);
    return -1;
  }
  write(fd, gpio, 2);
  close(fd);
  return 0;
}

int gpio_direction(unsigned int gpio) {
  int fd;
  char *sysFile;

  switch (gpio) {
  case 42:
    sysFile = SYSFS_GPIO_DIR GPIO42 "/direction";
    break;
  case 43:
    sysFile = SYSFS_GPIO_DIR GPIO43 "/direction";
    break;
  case 44:
    sysFile = SYSFS_GPIO_DIR GPIO44 "/direction";
    break;
  case 45:
    sysFile = SYSFS_GPIO_DIR GPIO45 "/direction";
    break;
  case 60:
    sysFile = SYSFS_GPIO_DIR GPIO60 "/direction";
    break;
  case 61:
    sysFile = SYSFS_GPIO_DIR GPIO61 "/direction";
    break;
  case 62:
    sysFile = SYSFS_GPIO_DIR GPIO62 "/direction";
    break;
  case 63:
    sysFile = SYSFS_GPIO_DIR GPIO63 "/direction";
    break;
  }
  fd = open(sysFile, O_WRONLY);
  write(fd, "out", 3);
  close(fd);
  return 0;
}

int gpio_value(unsigned int gpio, char *value) {
  int fd;
  char *sysFile;

  switch (gpio) {
  case 42:
    sysFile = SYSFS_GPIO_DIR GPIO42 "/value";
    break;
  case 43:
    sysFile = SYSFS_GPIO_DIR GPIO43 "/value";
    break;
  case 44:
    sysFile = SYSFS_GPIO_DIR GPIO44 "/value";
    break;
  case 45:
    sysFile = SYSFS_GPIO_DIR GPIO45 "/value";
    break;
  case 60:
    sysFile = SYSFS_GPIO_DIR GPIO60 "/value";
    break;
  case 61:
    sysFile = SYSFS_GPIO_DIR GPIO61 "/value";
    break;
  case 62:
    sysFile = SYSFS_GPIO_DIR GPIO62 "/value";
    break;
  case 63:
    sysFile = SYSFS_GPIO_DIR GPIO63 "/value";
    break;
  }
  fd = open(sysFile, O_WRONLY);
  if (fd <= 0) {
    printf("pgio_value error %s", sysFile);
    return -1;
  }
  write(fd, value, 1);
  close(fd);
  return 0;
}

int gpio_unexport(char *gpio) {
  int fd;
  fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
  write(fd, gpio, 2);
  close(fd);
  return 0;
}

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
  case 4:
    sysFile = SYSFS_PWM_DIR PWM67 "/export";
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
  case 4:
    sysFile = SYSFS_PWM_DIR PWM67 "/unexport";
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
  case 4:
    sysFile = SYSFS_PWM_DIR PWM67 "/pwm0/enable";
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
  case 4:
    sysFile = SYSFS_PWM_DIR PWM67 "/pwm0/period";
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
  case 4:
    sysFile = SYSFS_PWM_DIR PWM67 "/pwm0/duty_cycle";
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
  case 4:
    sysFile = SYSFS_PWM_DIR PWM67 "/pwm0/polarity";
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
  case 4:
    sysFile = SYSFS_PWM_DIR PWM67 "/pwm0/enable";
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

int init_gpio() {
  if (gpio_export("42") < 0) {
    return -1;
  }
  if (gpio_export("43") < 0) {
    return -1;
  }
  if (gpio_export("44") < 0) {
    return -1;
  }
  if (gpio_export("45") < 0) {
    return -1;
  }

  if (gpio_export("60") < 0) {
    return -1;
  }
  if (gpio_export("61") < 0) {
    return -1;
  }
  if (gpio_export("62") < 0) {
    return -1;
  }
  if (gpio_export("63") < 0) {
    return -1;
  }

  if (gpio_direction(42) < 0) {
    return -1;
  }
  if (gpio_direction(43) < 0) {
    return -1;
  }
  if (gpio_direction(44) < 0) {
    return -1;
  }
  if (gpio_direction(45) < 0) {
    return -1;
  }

  if (gpio_direction(60) < 0) {
    return -1;
  }
  if (gpio_direction(61) < 0) {
    return -1;
  }
  if (gpio_direction(62) < 0) {
    return -1;
  }
  if (gpio_direction(63) < 0) {
    return -1;
  }

  if (gpio_value(42, "0") < 0) {
    return -1;
  }
  if (gpio_value(43, "0") < 0) {
    return -1;
  }
  if (gpio_value(44, "0") < 0) {
    return -1;
  }
  if (gpio_value(45, "0") < 0) {
    return -1;
  }
  if (gpio_value(60, "0") < 0) {
    return -1;
  }
  if (gpio_value(61, "0") < 0) {
    return -1;
  }
  if (gpio_value(62, "0") < 0) {
    return -1;
  }
  if (gpio_value(63, "0") < 0) {
    return -1;
  }
}

void uninit_gpio() {
  gpio_unexport("42");
  gpio_unexport("43");
  gpio_unexport("44");
  gpio_unexport("45");
  gpio_unexport("60");
  gpio_unexport("61");
  gpio_unexport("62");
  gpio_unexport("63");
}

int init_pwm() {
  // 初始化3种颜色的灯为不发光
  for (int i = 1; i < 5; i++) {
    if (pwm_export(i) < 0) {
      return -1;
    }

    if (pwm_disable(i) < 0) {
      return -1;
    }
    if (pwm_config_period(i, my_period) < 0) {
      return -1;
    }
    my_duty = my_period * 0.01;
    if (pwm_config_duty_cycle(i, my_duty) < 0) {
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

int uninit_pwm() {
  for (int i = 1; i < 5; i++) {
    if (pwm_unexport(i) < 0) {
      return -1;
    }
  }
}

void at_signal(int sig) {
  uninit_gpio();
  uninit_pwm();
}

void at_exit(void) {
  uninit_gpio();
  uninit_pwm();
}

int main() {
  if (init_gpio() < 0) {
    printf("GPIO初始化失败");
    return -1;
  }
  if (init_pwm() < 0) {
    printf("pwm初始化失败");
    return -1;
  }

  while (1) {
    scanf("%d", &cmd);
    if (cmd == -1) {
      uninit_gpio();
      uninit_pwm();
      printf("exit\n");
      break;
    }

    if (cmd == 0) {
      printf("请输入功能代码:\n-1.退出\n0.说明\n1.前进\n2.停止\n3.后退\n4."
             "左移\n5.右移\n6.左转圈\n7.右转圈\n90-95调速\n");
    }

    if (cmd == 1) {
      if (gpio_value(42, "0") < 0) {
        return -1;
      }
      if (gpio_value(43, "1") < 0) {
        return -1;
      }
      if (gpio_value(44, "0") < 0) {
        return -1;
      }
      if (gpio_value(45, "1") < 0) {
        return -1;
      }

      if (gpio_value(60, "0") < 0) {
        return -1;
      }
      if (gpio_value(61, "1") < 0) {
        return -1;
      }
      if (gpio_value(62, "0") < 0) {
        return -1;
      }
      if (gpio_value(63, "1") < 0) {
        return -1;
      }
    }

    if (cmd == 2) {
      if (gpio_value(42, "0") < 0) {
        return -1;
      }
      if (gpio_value(43, "0") < 0) {
        return -1;
      }
      if (gpio_value(44, "0") < 0) {
        return -1;
      }
      if (gpio_value(45, "0") < 0) {
        return -1;
      }

      if (gpio_value(60, "0") < 0) {
        return -1;
      }
      if (gpio_value(61, "0") < 0) {
        return -1;
      }
      if (gpio_value(62, "0") < 0) {
        return -1;
      }
      if (gpio_value(63, "0") < 0) {
        return -1;
      }
    }

    if (cmd == 3) {
      my_duty = my_period * 0.3;
      for (int i = 1; i < 5; i++) {
        if (pwm_config_duty_cycle(i, my_duty) < 0) {
          return -1;
        }
      }

      if (gpio_value(42, "1") < 0) {
        return -1;
      }
      if (gpio_value(43, "0") < 0) {
        return -1;
      }
      if (gpio_value(44, "1") < 0) {
        return -1;
      }
      if (gpio_value(45, "0") < 0) {
        return -1;
      }

      if (gpio_value(60, "1") < 0) {
        return -1;
      }
      if (gpio_value(61, "0") < 0) {
        return -1;
      }
      if (gpio_value(62, "1") < 0) {
        return -1;
      }
      if (gpio_value(63, "0") < 0) {
        return -1;
      }
    }

    if (cmd == 4) {
      if (gpio_value(42, "0") < 0) {
        return -1;
      }
      if (gpio_value(43, "1") < 0) {
        return -1;
      }
      if (gpio_value(44, "1") < 0) {
        return -1;
      }
      if (gpio_value(45, "0") < 0) {
        return -1;
      }

      if (gpio_value(60, "0") < 0) {
        return -1;
      }
      if (gpio_value(61, "1") < 0) {
        return -1;
      }
      if (gpio_value(62, "1") < 0) {
        return -1;
      }
      if (gpio_value(63, "0") < 0) {
        return -1;
      }
    }

    if (cmd == 5) {
      if (gpio_value(42, "1") < 0) {
        return -1;
      }
      if (gpio_value(43, "0") < 0) {
        return -1;
      }
      if (gpio_value(44, "0") < 0) {
        return -1;
      }
      if (gpio_value(45, "1") < 0) {
        return -1;
      }

      if (gpio_value(60, "1") < 0) {
        return -1;
      }
      if (gpio_value(61, "0") < 0) {
        return -1;
      }
      if (gpio_value(62, "0") < 0) {
        return -1;
      }
      if (gpio_value(63, "1") < 0) {
        return -1;
      }
    }

    if (cmd == 6) {
      if (gpio_value(42, "1") < 0) {
        return -1;
      }
      if (gpio_value(43, "0") < 0) {
        return -1;
      }
      if (gpio_value(44, "0") < 0) {
        return -1;
      }
      if (gpio_value(45, "1") < 0) {
        return -1;
      }

      if (gpio_value(60, "0") < 0) {
        return -1;
      }
      if (gpio_value(61, "1") < 0) {
        return -1;
      }
      if (gpio_value(62, "1") < 0) {
        return -1;
      }
      if (gpio_value(63, "0") < 0) {
        return -1;
      }
    }

    if (cmd == 7) {
      if (gpio_value(42, "0") < 0) {
        return -1;
      }
      if (gpio_value(43, "1") < 0) {
        return -1;
      }
      if (gpio_value(44, "1") < 0) {
        return -1;
      }
      if (gpio_value(45, "0") < 0) {
        return -1;
      }

      if (gpio_value(60, "1") < 0) {
        return -1;
      }
      if (gpio_value(61, "0") < 0) {
        return -1;
      }
      if (gpio_value(62, "0") < 0) {
        return -1;
      }
      if (gpio_value(63, "1") < 0) {
        return -1;
      }
    }

    if (cmd == 90) {
      my_duty = my_period * 0.1;
      for (int i = 1; i < 5; i++) {
        if (pwm_config_duty_cycle(i, my_duty) < 0) {
          return -1;
        }
      }
    }

    if (cmd == 91) {
      my_duty = my_period * 0.3;
      for (int i = 1; i < 5; i++) {
        if (pwm_config_duty_cycle(i, my_duty) < 0) {
          return -1;
        }
      }
    }
    if (cmd == 92) {
      my_duty = my_period * 0.5;
      for (int i = 1; i < 5; i++) {
        if (pwm_config_duty_cycle(i, my_duty) < 0) {
          return -1;
        }
      }
    }
    if (cmd == 93) {
      my_duty = my_period * 0.7;
      for (int i = 1; i < 5; i++) {
        if (pwm_config_duty_cycle(i, my_duty) < 0) {
          return -1;
        }
      }
    }
    if (cmd == 94) {
      my_duty = my_period * 0.9;
      for (int i = 1; i < 5; i++) {
        if (pwm_config_duty_cycle(i, my_duty) < 0) {
          return -1;
        }
      }
    }
    if (cmd == 95) {
      my_duty = my_period * 1;
      for (int i = 1; i < 5; i++) {
        if (pwm_config_duty_cycle(i, my_duty) < 0) {
          return -1;
        }
      }
    }
  }
  return 0;
}