/*
 * rn8207.c
 *
 *  Created on: 2017年9月22日
 *      Author: Lzy
 */
#include "rn8207.h"
#include <math.h>

/**
 * 功  能：短延时
 */
static void rn8207_udelay(int t)
{
	usleep(t);
}

/**
 * 功  能：ms延时
 */
static void rn8207_mdelay(int t)
{
	rn8207_udelay(t*1000);
}

/**
 * 功  能：s延时
 */
static void rn8207_delay(int t)
{
	sleep(t);
}
/**
 * 功  能：串口发送
 */
static int rn_uart_sent(uchar *buf, int len)
{

	rn8207_mdelay(100);
	return len;
}

/**
 * 功  能：串口接收
 * 返  回：接收到的长度
 */
static int rn_uart_recv(uchar *buf)
{
	memset(buf,0,RN_DATA_LEN);

	rn8207_mdelay(80);

	return 4;
}

/**
 * 功 能：根据长度把字节转化为数据
 * 返 回：转化后的数据
 */
static int charToByLen(uchar *buf, int len)
{
	int data = 0;
	for(int i=0; i<len; ++i)
		data  = (data<<8) + buf[i];
	return data;
}

/**
 * 功 能：读取8207数据
 * 返 回：-1 出错
 */
static int recv_8207(rn_rDataFormat *pkt)
{
	static uchar buf[RN_DATA_LEN];
	int offset=0;

	memset(buf,0,sizeof(buf));
	buf[offset++]=pkt->id;
	buf[offset++]=pkt->cmd;

	int ret = rn_uart_sent(buf,offset);
	if(ret <= 0) {
		rn_debug("read_8207 rn_uart_sent err: %d\n", ret);
	}  else {
		ret = rn_uart_recv(buf);
		if(ret == pkt->len) {
			ret = pkt->data = charToByLen(buf, pkt->len);
		} else {
			ret = 0;
			rn_debug("read_8207 err: id=%d cmd=%x len=%d\n", pkt->id, pkt->cmd, pkt->len);
		}
	}

	return ret;
}

/**
 * 功 能：读取8207数据   另一种接口而已
 * 返 回：-1 出错
 */
static int read_8207(uchar id,uchar cmd,uchar len)
{
	static rn_rDataFormat pkt;

	pkt.id = id;
	pkt.cmd = cmd;
	pkt.len = len;

	return recv_8207(&pkt);
}

/**
 * 功 能：校验和
 * 		校验和字节；读操作由从机端发送，写操作由主机端发送校验和算法如下：
 * 		CheckSum[7:0] = ~(ChipID+CMD[7:0] + DATAn[7:0] + …… +DATA1[7:0])
 * 		即将CMD和数据相加，抛弃进位，最后的结果按位取反；
 */
static uchar rn_getCheckSum(uchar *buf, int len)
{
	char i,xy=0;
	for(i=0; i<len; ++i)
		xy+=buf[i];
	return ~xy;
}

/**
 * 功   能：发送命令给6720
 * 入口参数：pkt RN8207C UART通讯帧格结构体
 * 返  回：
 */
static int write_8207(rn_wDataFormat *pkt)
{
	static uchar buf[RN_DATA_LEN];
	int i,offset=0;

	memset(buf,0,sizeof(buf));
	buf[offset++] = pkt->id;
	buf[offset++] = pkt->cmd;

	for(i=0;i<pkt->len;i++)
		buf[offset++] = pkt->data[i];
	buf[offset++] = rn_getCheckSum(buf, pkt->len+2);

	int ret = rn_uart_sent(buf,pkt->len+3);
	if(ret <= 0)
		rn_debug("read_8207 write_8207 err: %d\n", ret);

	return ret;
}


// 蕊片片选地址表
static uchar addrTab[3] = {0x88, 0x99,0xAA};
static uchar curAcDcTab[3] = {1,1,1}; // 交直接标志位 1表示交流 0 表示直泫

/**
 * 功  能：获取交直流状态
 * 返  回：返回 1表示交流 0 表示直泫
 */
static int rn_getAcDcStatus(int id)
{
	rn_rDataFormat pkt;

	pkt.id = addrTab[id];
	pkt.cmd = 0x41;
	pkt.len = 1;

	int ret = recv_8207(&pkt);
	if(ret & 0x20) {
		ret = 1;            // ac 交流
	} else {
		ret = 0;            // dc 直流
	}

	return ret;
}

// 直流默认配置参数
char register_dc_DCIH[6];
char register_dc_DCL[6];
int IARMSOS_reg[3];

/**
 * 功 能：直流默认配置
 */
static void rn_dc_set(int i)
{
	char buf[RN_DATA_LEN];
	rn_wDataFormat pkt;
	pkt.data = buf;

	pkt.id = addrTab[i];
	pkt.cmd = 0xEA; // 特殊命令
	buf[0] = 0xE5; // 写使能命令
	pkt.len = 1;
	write_8207(&pkt);

	pkt.cmd = 0x82;
	buf[0] = 0x16;                          // H
	buf[1] = 0xCA;                          // L
	pkt.len = 2;
	write_8207(&pkt);

	pkt.cmd = 0x80;
	buf[0] = 0x16;                          // H
	buf[1] = 0x00;                          // L
	pkt.len = 2;
	write_8207(&pkt);

	pkt.cmd = 0x81;
	buf[0] = 0xB0;                          // H
	buf[1] = 0x63;                          // L
	pkt.len = 2;
	write_8207(&pkt);  //关闭高通滤波

	pkt.cmd = 0x93;
	buf[0] = register_dc_DCIH[i*2];
	buf[1] = register_dc_DCIH[i*2+1];
	pkt.len = 2;
	write_8207(&pkt);  //关闭高通滤波

	pkt.cmd = 0x96;
	buf[0] = 0x00;
	buf[1] = register_dc_DCL[i*2];
	pkt.len = 2;
	write_8207(&pkt);  //关闭高通滤波

	pkt.cmd = 0x96;
	buf[0] = register_dc_DCL[i*2+1];
	buf[1] = 0;
	pkt.len = 2;
	write_8207(&pkt);  //关闭高通滤波

	pkt.cmd = 0x96;
	buf[0] = IARMSOS_reg[i] / 256;
	buf[1] = IARMSOS_reg[i] % 256;
	pkt.len = 2;
	write_8207(&pkt);  //关闭高通滤波

	pkt.cmd = 0xEA; // 特殊命令
	buf[0] = 0xDC;
	pkt.len = 1;
	write_8207(&pkt);
}

// 交流默认配置参数
char prin_com[3]={0,0,0};
int register_GPQ[4]={0,0,0};
char register_Phs[4]={0,0,0};

/**
 * 功 能：交流默认配置
 */
static void rn_ac_set(int i)
{
	char buf[RN_DATA_LEN];
	rn_wDataFormat pkt;
	pkt.data = buf;

	pkt.id = addrTab[i];
	pkt.cmd = 0xEA; // 特殊命令
	buf[0] = 0xE5; // 写使能命令
	pkt.len = 1;
	write_8207(&pkt);

	pkt.cmd = 0x82;
	buf[0] = 0x16;                          // H
	buf[1] = 0x2B;                          // L
	pkt.len = 2;
	write_8207(&pkt);

	int ret = read_8207(pkt.id, 0x02, 2);
	if(ret >= 0) {
		prin_com[i] = ret%256;
	} else {
		// 出错
	}

	pkt.cmd = 0x80;   //  一倍增益
	buf[0] = 0x16;                          // H
	buf[1] = 0x00;                          // L
	pkt.len = 2;
	write_8207(&pkt);

	pkt.cmd = 0x85;
	buf[0] =register_GPQ[i]/256%256;                         //      0XF8
	buf[1] =register_GPQ[i]%256;           //   0XFB
	pkt.len = 2;
	write_8207(&pkt);

	pkt.cmd = 0x87;
	buf[0] = register_Phs[i];
	pkt.len = 1;
	write_8207(&pkt);

	pkt.cmd = 0x81;
	buf[0] = 0xB0;                 // 0xB0 ：10110000
	buf[1] = 0x03;                //0x03 ：00000011
	pkt.len = 2;
	write_8207(&pkt);

	pkt.cmd = 0xEA; // 特殊命令
	buf[0] = 0xDC;
	pkt.len = 1;
	write_8207(&pkt);
}


/**
 * 功 能：默认配置
 */
static void correct_set()
{
	int i,ret, line = LINE_NUM; // 相数
	for(i=0; i<line; ++i)
	{
		ret = curAcDcTab[i] = rn_getAcDcStatus(i);
		if(ret) {
			rn_ac_set(i);
		} else {
			rn_dc_set(i);
		}
	}
}


/**
 * 功  能：初始化蕊片 使其软复位
 */
void init_8207(void)
{
	uchar buf[RN_DATA_LEN];
	rn_wDataFormat pkt;

	pkt.data = buf;
	pkt.len = 1;
	pkt.cmd = 0xEA; // 特殊命令

	int i,line = LINE_NUM; // 相数
	for(i=0; i<line; ++i) {
		pkt.id = addrTab[i];
		buf[0] = 0xE5; // 写使能命令
		write_8207(&pkt);

		buf[0] = 0xFA; // 软件复位命令
		write_8207(&pkt);
		buf[0] = 0xDC; // 写保护命令
		write_8207(&pkt);
	}
	rn8207_mdelay(300);
	correct_set();
	rn8207_delay(1);
}


/**
 * 功  能：连续读二次，取其中最小的一个值
 * 说  明：怕有时通讯出问题，值出错出现错误大值
 */
static int rn8207_readTwo_fun(int line, int cmd, int len)
{
	int temp1 = read_8207(addrTab[line],cmd,len);
	int temp2 = read_8207(addrTab[line],cmd,len);
	int ret = RN_MIN(temp1, temp2);
	if(ret < 0)	ret = ~ret + 1;

	return ret;
}

// 实时电流、电压、功率值
int gVoltage[3]={0,0,0};
int gCurrent[3]={0,0,0};
int gPower[4]={0,0,0}; //有功功率值

/**
 * 功  能：读取电压、电流、功率
 */
static void rn8207_read_line(int line)
{
	gVoltage[line] = rn8207_readTwo_fun(line, 0x24, 3);
	gCurrent[line] = rn8207_readTwo_fun(line, 0x22, 3);
	gPower[line] = rn8207_readTwo_fun(line, 0x26,4);
}


// 电流、电压校准值
int gCurrentK[3]={0,0,0};
int gVoltageK[3]={0,0,0};

/**
 * 功  能：校准时读取电压、电流、功率
 */
static void rn8207_check_Line(int line)
{
	gVoltageK[line]=rn8207_readTwo_fun(line,0x24,3);
	gCurrentK[line]=rn8207_readTwo_fun(line,0x22,3);

	/////==========
	/// 保存校准值 gVoltageK[line] gCurrentK[line]

}


/**
 * 功 能：寄存器清0
 */
static void clear_zero_register(int line)
{
	uchar buf[RN_DATA_LEN];
	rn_wDataFormat pkt;

	pkt.data = buf;
	pkt.id = addrTab[line];

	pkt.cmd = 0xEA; // 特殊命令
	buf[0] = 0xE5; // 写使能命令
	pkt.len = 1;
	write_8207(&pkt);

	pkt.cmd = 0x85;
	buf[0] = 0;
	buf[1] = 0;
	pkt.len = 2;
	write_8207(&pkt);

	pkt.cmd = 0x87;
	buf[0] = 0;
	pkt.len = 1;
	write_8207(&pkt);

	pkt.cmd = 0xEA;
	buf[0] = 0xDC;
	pkt.len = 1;
	write_8207(&pkt);
}


static double Kp=0.000083;
static double gpa[4]={0,0,0},P_value[4]={0,0,0};

/**
 * 功 能：取读默认功率因素值
 */
static void read_correctPF(int line)
{
	rn8207_read_line(line);
	gpa[line] = -((gPower[line]*Kp-1320)/1320)/(1+(gPower[line]*Kp-1320)/1320);           //功率增益
	if(gpa[line]<0)
		register_GPQ[line] = gpa[line]*32768+65536;
	else
		register_GPQ[line] = gpa[line]*32768;

	uchar buf[RN_DATA_LEN];
	rn_wDataFormat pkt;

	pkt.data = buf;
	pkt.id = addrTab[line];

	pkt.cmd = 0xEA; // 特殊命令
	buf[0] = 0xE5; // 写使能命令
	pkt.len = 1;
	write_8207(&pkt);

	int data_aa  = read_8207(addrTab[line],0x05,2);

	pkt.cmd = 0x85;
	buf[0] = register_GPQ[line]/256%256;
	buf[1] = register_GPQ[line]%256;
	pkt.len = 2;
	write_8207(&pkt);

	pkt.cmd = 0xEA;
	buf[0] = 0xDC;
	pkt.len = 1;
	write_8207(&pkt);

	/////==========
	/// 保存 register_GPQ[line]
}




static void read_correctPHx(int line)
{
	rn8207_read_line(line);
	if(register_GPQ[line]>32768)
		P_value[line] = (float)(register_GPQ[line]-65536)/32768;             //归一化值
	else
		P_value[line] =(float)register_GPQ[line]/32768;

	if((asin(-(gPower[line]*Kp*(1+P_value[line])-660)/660/1.732)/0.02)>0)                    //相位寄存器值
		register_Phs[line] = (asin(-(gPower[line]*Kp*(1+P_value[line])-660)/660/1.732)/0.02);
	else
		register_Phs[line] = (asin(-(gPower[line]*Kp*(1+P_value[line])-660)/660/1.732)/0.02) + 256;

	uchar buf[RN_DATA_LEN];
	rn_wDataFormat pkt;

	pkt.data = buf;
	pkt.id = addrTab[line];

	pkt.cmd = 0xEA; // 特殊命令
	buf[0] = 0xE5; // 写使能命令
	pkt.len = 1;
	write_8207(&pkt);

	/////=========
	int data_aax = read_8207(addrTab[line],0x07,1);

	pkt.cmd = 0x87;
	buf[0] =register_Phs[line];
	pkt.len = 1;
	write_8207(&pkt);

	pkt.cmd = 0xEA;
	buf[0] = 0xDC;
	pkt.len = 1;
	write_8207(&pkt);

	/////==========
	/// 保存 register_Phs[line]
}


char correct_flag_ac[3]={0,0,0};  //交流校准标志
char correct_flag_phase_ac[3]={0,0,0};  //交流相位校准标志
char register_offset_dc[4]={0,0,0,0};   //直流偏置校准

//交流校准
void correct_ac_parameter(int line)
{
	if(correct_flag_ac[line]==1)             //              校准
	{
		clear_zero_register(line);
		rn8207_check_Line(line);
		read_correctPF(line);
		correct_flag_ac[line]=2;
	}
	else if(correct_flag_phase_ac[line]==1)                  //PHxA
	{
		read_correctPHx(line);
		correct_flag_phase_ac[line] = 2;
	}
}




