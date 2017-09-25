/*
 * rn_debug.h
 *
 *  Created on: 2017年9月22日
 *      Author: Lzy
 */

#ifndef RN8207_RNDEBUG_H_
#define RN8207_RNDEBUG_H_
#include <string.h>
#include <strings.h>	/* for strcasecmp() */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


#define TRUE RT_TRUE
#define FALSE RT_FALSE
#define true TRUE
#define false FALSE

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;


#define     DEBUG_SWITCH    1       /* 打开调试信息打印功能 */
#define     ERR_DEBUG_SWITCH    1   /* 打印错误信息打印功能 */
#define   EXAM_ASSERT_TEST_        1    /* 开启断言 */

/**
 * 简单打印调试信息
 */
#if    DEBUG_SWITCH
#define rn_debug(fmt,args...) printf(fmt, ##args)
#else
#define rn_debug(fmt,args...) /*do nothing */
#endif

/**
 * 错误信息打印
 * 自动打印发生错误时代码所在的位置
 */
#if    ERR_DEBUG_SWITCH
#define rn_errDebug(fmt,args...) printf("\nError:\nFile:<%s> Fun:[%s] Line:%d\n "fmt, __FILE__, __FUNCTION__, __LINE__, ##args)
#else
#define rn_errDebug(fmt,args...) /*do nothing */
#endif



#endif /* RN8207_RNDEBUG_H_ */
