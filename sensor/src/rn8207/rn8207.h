/*
 * rn8207.h
 *
 *  Created on: 2017年9月22日
 *      Author: Lzy
 */

#ifndef RN8207_RN8207_H_
#define RN8207_RN8207_H_
#include "rndebug.h"

#define LINE_NUM  3  // 3相
#define RN_DATA_LEN 32 // 缓冲区长度

#define RN_MIN(x,y)	((x)<(y)?(x):(y))

/**
 * RN8207C UART写通讯帧格式
 */
typedef struct _rn_wDataFormat {
	uchar id; // 片选地址字节，由主机端发送；若主机发送的片选地址为11，表示广播地址，
	uchar cmd; // 命令字节，由主机端发送，
	uchar len; // 数据长度
	uchar *data; // 数据字节；读操作由从机端发送，写操作由主机端发送
	uchar cksm; // 校验和字节；读操作由从机端发送，写操作由主机端发送
}rn_wDataFormat;

/**
 * RN8207C UART读通讯帧格式
 */
typedef struct _rn_rDataFormat {
	uchar id; // 片选地址字节，由主机端发送；若主机发送的片选地址为11，表示广播地址，
	uchar cmd; // 命令字节，由主机端发送，
	uchar len; // 数据长度
	int data; // 数据字节；读操作由从机端发送，写操作由主机端发送
	uchar cksm; // 校验和字节；读操作由从机端发送，写操作由主机端发送
}rn_rDataFormat;

#endif /* RN8207_RN8207_H_ */
