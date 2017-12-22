/*
 * main_hdc.c
 *
 *  Created on: 2017年9月22日
 *      Author: Lzy
 */
#include "i2c_hdc.h"


/**
 * 功 能：初始化传感器，打开传感器并获得句柄
 * 返 回：句柄数据的首地址
 */
int *init_sensor(void)
{
	static int *th = NULL;

	if(th == NULL) {
		th = (int *)malloc(sizeof(int) *TH_FN_NUM);
		if(th) {
			// 打开传感器
			th[0] = TH_open(TH_FN_1);
			th[1] = TH_open(TH_FN_2);
		} else {
			printf("init sensor malloc err!! \n");
		}
	}

	return th;
}

/**
 * 功 能：读取传感器数据
 * 出口参数：
 * 		pTem   温度缓冲区地址
 * 		pHum   湿度缓冲区地址
 */
void read_sensor(double *pTem, double *pHum)
{
	int i=0;
	int *fdArray = init_sensor();

	for(i=0; i<TH_FN_NUM; ++i)
	{
		int fd = fdArray[i];
		if(fd > 0)
		{
			double tem =TH_getTem(fd);
			if(tem > 0) {
#if  DEBUG_SWITCH
				printf("sensor %d temperature: %.3f C\n", i+1, tem);
#endif
			}
			pTem[i] = tem;

			double hum = TH_getHum(fd);
			if(hum > 0) {
#if  DEBUG_SWITCH
				printf("sensor %d humidity:    %.3f %%RH\n\n",i+1, hum);
#endif
			}
			pHum[i] = hum;
		}
	}
}


void main_hdc()
{
	double  temperature[TH_FN_NUM];  // 温度
	double  humidity[TH_FN_NUM];     // 湿度

	while(1)
	{
		sleep(1);
		read_sensor(temperature, humidity);
	}
}


