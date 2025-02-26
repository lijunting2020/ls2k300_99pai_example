# ls2k300_99pai_example

## 简介
这是b站“码工2020”视频合集“龙芯2k0300入坑指南”系统视频中用的资料和源代码。
##代码说明
###mod_driver：驱动实验代码
####01_helloworld：第一个驱动-必需要素
####02_param：加载驱动时传参
####03_dev_t：模块引用（符号导出）
####04_cdev：申请字符设备
####05_file：注册字符设备
####06_usr：功能调用（file_operations）
####07_private_data：私有数据使用
####08_private_data_test：内核空间与用户空间数据交互
####09_misc：杂项设备
####11_atomic：并发与竞争-原子变量操作
####12_spinlock：并发与竞争-自旋锁使用
####13_spinlock01：并发与竞争-自旋锁死锁实验
####14_sem：并发与竞争-信号量使用
####15_mutex：并发与竞争-互斥锁

####20_time：系统定时器-基本使用
####21_time01：系统定时器-循环定时
####22_time：系统定时器-以秒计时
####23_llseek：数据流定位函数实现
####24_ioctl01：设备控制-控制命令定义
####25_ioctl02：设备控制-基本控制实现
####26_ioctl03：设备控制-地址传参
####27_ioctl04：设备控制-定时器控制

##S_LED驱动模块使用
###1.适用99派wifi版
###2.下截mod_driver/s_led_dev/目录下的s_led.ko和op_led两个文件到开发板。假设已经将这两个文件下截到开发板的/root/目录下。
###3.远程登录开发板（SSH),以下步骤在开发板上完成。
###4.cd /etc/systemd/system
###5.nano s_led.service
###6.输入或复制如下内容：

Description=Load s_led Kernel Module

After=network.target

[Service]

Type=oneshot

ExecStart=/sbin/insmod /root/s_led.ko

ExecStart=/root/op_led 2

ExecStop=/sbin/rmmod s_led.ko

RemainAfterExit=yes

[Install]

WantedBy=multi-user.target

###7.按ctrl+O
###8.按ctrl+X
###9.systemctl start s_led
###10.systemctl enable s_led
###11.重启开发板。