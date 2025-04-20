#表示把目标文件helloworld.o作为模块进行编译。obj是object的缩写，-m表示编译成模块
obj-m += file.o
#使用绝对路径的方式指定内核源码的顶层路径
KDIR := /home/lijunting/aown/qianrushi/99pai/os/kernel_src/linux-6.9
#获取本文件(Makefile)的路径
PWD ?= $(shell pwd)
all:
    #进到KDIR指定的目录，使用PWD目录下的源码和本Makefile文件编译驱动模块,make命令前需要加TAB符，而不是空格
	make -C $(KDIR) M=$(PWD)
clean:
	#清除编译文件
	rm -f *.cmd *.symvers *.order *.ko *.mod *.o .*.cmd *.mod.c *.d