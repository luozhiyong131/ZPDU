/*
 * i2c_hdc.c
 * 温湿度传感 操作实现文件
 *
 *  Created on: 2017年9月19日
 *      Author: Lzy
 */
#include "i2c_hdc.h"

/**
 * 功能：打开设备文件
 * 返回：设备句柄  <0 出错
 */
int TH_open(char *fn)
{
	int fd=open(fn, O_RDWR);
	if(fd < 0)
		perror("open dev file \n");
	return fd;
}

/**
 * 功  能：根据寄存器地址，读相应的的值
 * 入口参数：fd 文件句柄
 * 			addr 器件地址
 * 			reg 寄存器地址
 * 返  回：0xffff 为无效值
 */
static ushort i2c_read(int fd, uchar addr, uchar reg)
{
	int ret=0;
	uchar buf[4]={0};
	struct i2c_msg             msgs;
	struct i2c_rdwr_ioctl_data msgset;

	msgset.msgs = &msgs;
	msgset.nmsgs = 1;

	buf[0] = reg;
	msgs.addr = addr;
	msgs.flags = 0;
	msgs.buf = (void *)buf;
	msgs.len = 1;

	ret = ioctl(fd, I2C_RDWR, &msgset);
	if (ret<0) {
		printf("i2c setenv address faile ret: %x \n", ret);
		return 0xffff;
	}
	usleep(50*1000);

	msgs.addr = addr;
	msgs.flags = I2C_M_RD;
	msgs.buf = (void *)buf;
	msgs.len = 2;

	ret = ioctl(fd, I2C_RDWR, &msgset);
	if (ret<0) {
		printf("i2c getenv address faile ret: %x \n", ret);
		return 0xffff;
	}

	return buf[0]*256 + buf[1];
}


/**
 * 功  能：获取温度值
 * 入口参数：fd 文件句柄
 * 返  回：温度值  <0 出错
 */
double TH_getTem(int fd)
{
	double value = -1;
	ushort ret = i2c_read(fd, I2C_ADDR,0);
	if(ret != 0xffff)
		value = ret * (165.0/65536.0) - 40;

	return value;
}


/**
 * 功  能：获取湿度值
 * 入口参数：fd 文件句柄
 * 返  回：湿度值  <0 出错
 */
double TH_getHum(int fd)
{
	double value = -1;
	ushort ret = i2c_read(fd, I2C_ADDR,1);
	if(ret != 0xffff)
		value = ret * (100.0/65536.0);

	return value;
}
