/*
 ============================================================================
 Name        : sensor.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include "sht3x.h"

//-----------------------------------------------------------------------------
static u8t ReadUserButton(void)          /* -- adapt this code for your platform -- */
{
	return 1;
}

//-----------------------------------------------------------------------------
etError read_value(int th, ft *temperature, ft *humidity)
{
	etError   error;       // error code
	regStatus status;      // sensor status
	bt        heater;      // heater, false: off, true: on

	error = NO_ERROR;
	set_i2c_num(th);

	// loop while no error
	while(error == NO_ERROR)
	{
		// read status register
		error |= SHT3X_ReadStatus(&status.u16);
		if(error != NO_ERROR) break;

		// check if the reset bit is set after a reset or power-up
		if(status.bit.ResetDetected)
		{
			//override default temperature and humidity alert limits (red LED)
			error = SHT3X_SetAlertLimits( 70.0f,  50.0f,  // high set:   RH [%], T [癈]
					68.0f,  48.0f,  // high clear: RH [%], T [癈]
					32.0f,  -2.0f,  // low clear:  RH [%], T [癈]
					30.0f,  -4.0f); // low set:    RH [%], T [癈]
			if(error != NO_ERROR) break;


			// clear reset and alert flags
			error = SHT3X_ClearAllAlertFlags();
			if(error != NO_ERROR) break;

			//start periodic measurement, with high repeatability and 1 measurements per second
			error = SHT3X_StartPeriodicMeasurment(REPEATAB_HIGH, FREQUENCY_1HZ);
			if(error != NO_ERROR) break;
		}

		// read measurment buffer
		error = SHT3X_ReadMeasurementBuffer(temperature, humidity);
		if(error == NO_ERROR)
		{
			// flash blue LED to signalise new temperature and humidity values
			DelayMicroSeconds(10000);
			break;
		}
		else if (error == ACK_ERROR)
		{
			error = NO_ERROR;
		}
		else break;

		// read heater status
		heater = status.bit.HeaterStatus ? TRUE : FALSE;

		// if the user button is not pressed ...
		if(ReadUserButton() == 0)
		{
			// ... and the heater is on
			if(heater)
			{
				// switch off the sensor internal heater
				error |= SHT3X_DisableHeater();
				if(error != NO_ERROR) break;
			}
		}
		else
			// if the user button is pressed ...
		{
			// ... and the heater is off
			if(!heater)
			{
				// switch on the sensor internal heater
				error |= SHT3X_EnableHeater();
				if(error != NO_ERROR) break;
			}
		}

		// wait 100ms
		DelayMicroSeconds(100000);
	}

	if(error != NO_ERROR)
	{
		// in case of an error ...

		// ... try first a soft reset ...
		error = SHT3X_SoftReset();

		// ... if the soft reset fails, do a hard reset
		if(error != NO_ERROR)
		{
			SHT3X_HardReset();
		}
	}

	// flash green LED to signalise an error
	DelayMicroSeconds(10000);

	return error;
}

//-----------------------------------------------------------------------------
void init_sensor(int th)
{
	set_i2c_num(th);
	SHT3X_Init(0x45); // Address: 0x44 = Sensor on EvalBoard connector
	//          0x45 = Sensor on EvalBoard
}

//-----------------------------------------------------------------------------
etError read_sensor(int th, ft *temperature, ft *humidity)
{
	etError   error;       // error code
	u32t      serialNumber;// serial number

	// wait 50ms after power on
	DelayMicroSeconds(50000);

	set_i2c_num(th);
	error = SHT3x_ReadSerialNumber(&serialNumber);
	if(error == NO_ERROR) // do error handling here
	{
		error = read_value(th, temperature, humidity);

		// demonstrate a single shot measurement with clock-stretching
		//	error = SHT3X_GetTempAndHumi(temperature, humidity, REPEATAB_HIGH, MODE_CLKSTRETCH, 50);
		//	if(error != NO_ERROR){} // do error handling here

		// demonstrate a single shot measurement with polling and 50ms timeout
		//	error = SHT3X_GetTempAndHumi(temperature, humidity, REPEATAB_HIGH, MODE_POLLING, 50);
		//	if(error != NO_ERROR){} // do error handling here
	}

	return error;
}

int main(void) {
	ft        temperature[2]; // temperature [癈]
	ft        humidity[2];    // relative humidity [%RH]
	int i;

	for(i=0; i<2; ++i) {
		init_sensor(i);
	}

	while(1)
	{
		for(i=0; i<1; ++i)
			read_sensor(i,&temperature[i], &humidity[i]);
		DelayMicroSeconds(10000);
	}

//	test();
	return EXIT_SUCCESS;
}

