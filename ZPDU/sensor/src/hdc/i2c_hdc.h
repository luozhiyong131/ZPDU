/*
 * i2c_hdc.h
 *
 *  Created on: 2017年9月19日
 *      Author: Lzy
 */

#ifndef I2C_HDC_H_
#define I2C_HDC_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>	/* for NAME_MAX */
#include <sys/ioctl.h>
#include <string.h>
#include <strings.h>	/* for strcasecmp() */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>

#define DEBUG_SWITCH    1       /* 打开调试信息打印功能 */

#define I2C_ADDR  0x40  // 器件地址

/*设备文件名*/
#define TH_FN_1 "/dev/i2c-0"
#define TH_FN_2 "/dev/i2c-1"

#define TH_FN_NUM 2 // 传感器数量

// 数据类型定义
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned short ushort;

// 接口函数
int TH_open(char *fn);
double TH_getTem(int fd);
double TH_getHum(int fd);

#endif /* I2C_HDC_H_ */
