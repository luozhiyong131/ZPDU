//=============================================================================
//    S E N S I R I O N   AG,  Laubisruetistr. 50, CH-8712 Staefa, Switzerland
//=============================================================================
// Project   :  SHT3x Sample Code (V1.1)
// File      :  i2c_hal.c (V1.1)
// Author    :  RFU
// Date      :  6-Mai-2015
// Controller:  STM32F100RB
// IDE       :  �Vision V5.12.0.0
// Compiler  :  Armcc
// Brief     :  I2C hardware abstraction layer
//=============================================================================

//-- Includes -----------------------------------------------------------------
#include "i2c_hal.h"

//-- Defines ------------------------------------------------------------------
// I2C IO-Pins                        /* -- adapt the defines for your uC -- */

// SDA on port B, bit 14
//#define SDA_LOW()  (GPIOB->BSRR = 0x40000000) // set SDA to low
//#define SDA_OPEN() (GPIOB->BSRR = 0x00004000) // set SDA to open-drain
//#define SDA_READ   (GPIOB->IDR  & 0x4000)     // read SDA
//
//// SCL on port B, bit 13              /* -- adapt the defines for your uC -- */
//#define SCL_LOW()  (GPIOB->BSRR = 0x20000000) // set SCL to low
//#define SCL_OPEN() (GPIOB->BSRR = 0x00002000) // set SCL to open-drain
//#define SCL_READ   (GPIOB->IDR  & 0x2000)     // read SCL

#define I2C1_SCL_GPIO     114
#define I2C1_SDA_GPIO     113
#define I2C2_SCL_GPIO     116
#define I2C2_SDA_GPIO     115


//-- Static function prototypes -----------------------------------------------
static etError I2c_WaitWhileClockStreching(u8t timeout);

static int i2cNum = 0;
static int ioSdaArray[4] = {I2C1_SDA_GPIO, I2C2_SDA_GPIO};
static int ioSclArray[4] = {I2C1_SCL_GPIO, I2C2_SCL_GPIO};

//-----------------------------------------------------------------------------
void set_i2c_num(int num)
{
	i2cNum = num;
}

//-----------------------------------------------------------------------------
static void i2c_out(int io, int level)
{
	static char cmd[64] = {0};

	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd,"echo \"out\" > /sys/class/gpio/gpio%d/direction", io);
	system(cmd);

	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd,"echo \"%d\" > /sys/class/gpio/gpio%d/value",level, io);
	system(cmd);
}

//-----------------------------------------------------------------------------
static int i2c_input(int io)
{
	static char cmd[64] = {0};

	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd,"echo \"in\" > /sys/class/gpio/gpio%d/direction", io);
	system(cmd);
	DelayMicroSeconds(1);

	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd,"cat /sys/class/gpio/gpio%d/value", io);
	int ret = system(cmd);
	if(ret < 0) {
		printf("i2c read Err: i2c num:%d \n", io);
		ret = 0;
	}
	return ret;

	return 0;
}

//-----------------------------------------------------------------------------
//void SDA_LOW()
//{
//	i2c_out(ioSdaArray[i2cNum], 0);
//}

#define SDA_LOW() i2c_out(ioSdaArray[i2cNum], 0)

//-----------------------------------------------------------------------------
//void SDA_OPEN()
//{
//	i2c_out(ioSdaArray[i2cNum], 1);
//}

#define SDA_OPEN() i2c_out(ioSdaArray[i2cNum], 1)

//-----------------------------------------------------------------------------
//int SDA_READ()
//{
//	return i2c_input(ioSdaArray[i2cNum]);
//}

#define SDA_READ   (i2c_input(ioSdaArray[i2cNum]))
//-----------------------------------------------------------------------------
//void SCL_LOW()
//{
//	i2c_out(ioSclArray[i2cNum], 0);
//}

#define SCL_LOW()  i2c_out(ioSclArray[i2cNum], 0)
//-----------------------------------------------------------------------------
//void SCL_OPEN()
//{
//	i2c_out(ioSclArray[i2cNum], 1);
//}
#define SCL_OPEN() i2c_out(ioSclArray[i2cNum], 1)
//-----------------------------------------------------------------------------
//int SCL_READ()
//{
//	return i2c_input(ioSclArray[i2cNum]);
//}

#define SCL_READ i2c_input(ioSclArray[i2cNum])

//-----------------------------------------------------------------------------
void I2c_Init(void)                      /* -- adapt the init for your uC -- */
{
	int ioArray[4] = {I2C1_SCL_GPIO, I2C1_SDA_GPIO, I2C2_SCL_GPIO, I2C2_SDA_GPIO};
	char cmd[32] = {0}, i;

	for(i=0; i<4; ++i) {
		memset(cmd, 0, sizeof(cmd));
		sprintf(cmd, "echo %d > /sys/class/gpio/export", ioArray[i]);
		system(cmd);
	}

	for(i=0; i<4; ++i) {
		memset(cmd, 0, sizeof(cmd));
		sprintf(cmd,"echo \"out\" > /sys/class/gpio/gpio%d/direction", ioArray[i]);
		system(cmd);
	}
}

//-----------------------------------------------------------------------------
void I2c_StartCondition(void)
{
  SDA_OPEN();
  DelayMicroSeconds(1);

  SCL_OPEN();
  DelayMicroSeconds(1);

  SDA_LOW();
  DelayMicroSeconds(10);  // hold time start condition (t_HD;STA)

  SCL_LOW();
  DelayMicroSeconds(10);
}

//-----------------------------------------------------------------------------
void DelayMicroSeconds(u32t nbrOfUs)   /* -- adapt this delay for your uC -- */
{
	usleep(10 *1000);
}

//-----------------------------------------------------------------------------
void I2c_StopCondition(void)
{
  SCL_LOW();
  DelayMicroSeconds(1);
  SDA_LOW();
  DelayMicroSeconds(1);
  SCL_OPEN();
  DelayMicroSeconds(10);  // set-up time stop condition (t_SU;STO)
  SDA_OPEN();
  DelayMicroSeconds(10);
}

//-----------------------------------------------------------------------------
etError I2c_WriteByte(u8t txByte)
{
  etError error = NO_ERROR;
  u8t     mask;
  for(mask = 0x80; mask > 0; mask >>= 1)// shift bit for masking (8 times)
  {
    if((mask & txByte) == 0) SDA_LOW(); // masking txByte, write bit to SDA-Line
    else                     SDA_OPEN();
    DelayMicroSeconds(1);               // data set-up time (t_SU;DAT)
    SCL_OPEN();                         // generate clock pulse on SCL
    DelayMicroSeconds(5);               // SCL high time (t_HIGH)
    SCL_LOW();
    DelayMicroSeconds(1);               // data hold time(t_HD;DAT)
  }
  SDA_OPEN();                           // release SDA-line
  SCL_OPEN();                           // clk #9 for ack
  DelayMicroSeconds(1);                 // data set-up time (t_SU;DAT)
  if(SDA_READ) error = ACK_ERROR;       // check ack from i2c slave
  SCL_LOW();
  DelayMicroSeconds(20);                // wait to see byte package on scope
  return error;                         // return error code
}

//-----------------------------------------------------------------------------
etError I2c_ReadByte(u8t *rxByte, etI2cAck ack, u8t timeout)
{
  etError error = NO_ERROR;
  u8t mask;
  *rxByte = 0x00;
  SDA_OPEN();                            // release SDA-line
  for(mask = 0x80; mask > 0; mask >>= 1) // shift bit for masking (8 times)
  { 
    SCL_OPEN();                          // start clock on SCL-line
    DelayMicroSeconds(1);                // clock set-up time (t_SU;CLK)
    error = I2c_WaitWhileClockStreching(timeout);// wait while clock streching
    DelayMicroSeconds(3);                // SCL high time (t_HIGH)
    if(SDA_READ) *rxByte |= mask;        // read bit
    SCL_LOW();
    DelayMicroSeconds(1);                // data hold time(t_HD;DAT)
  }
  if(ack == ACK) SDA_LOW();              // send acknowledge if necessary
  else           SDA_OPEN();
  DelayMicroSeconds(1);                  // data set-up time (t_SU;DAT)
  SCL_OPEN();                            // clk #9 for ack
  DelayMicroSeconds(5);                  // SCL high time (t_HIGH)
  SCL_LOW();
  SDA_OPEN();                            // release SDA-line
  DelayMicroSeconds(20);                 // wait to see byte package on scope
  
  return error;                          // return with no error
}

//-----------------------------------------------------------------------------
etError I2c_GeneralCallReset(void)
{
  etError error;
  
  I2c_StartCondition();
                        error = I2c_WriteByte(0x00);
  if(error == NO_ERROR) error = I2c_WriteByte(0x06);
  
  return error;
}

//-----------------------------------------------------------------------------
static etError I2c_WaitWhileClockStreching(u8t timeout)
{
  etError error = NO_ERROR;
  
  while(SCL_READ == 0)
  {
    if(timeout-- == 0) return TIMEOUT_ERROR;
    DelayMicroSeconds(1000);
  }
  
  return error;
}
