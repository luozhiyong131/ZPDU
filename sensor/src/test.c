/*
 * test.c
 *
 *  Created on: 2017年9月8日
 *      Author: Lzy
 */
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include "sht3x.h"

/**
 * IO口测试
 */
int test(void) {
	int io[5] = {114, 113, 116, 115, 129};
	int len = 5, i;
	char cmd[32] = {0};

	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */

	for(i=0; i<len; ++i) {
		memset(cmd, 0, sizeof(cmd));
		sprintf(cmd, "echo %d > /sys/class/gpio/export", io[i]);
		system(cmd);
	}

	for(i=0; i<len; ++i) {
		memset(cmd, 0, sizeof(cmd));
		sprintf(cmd,"echo \"out\" > /sys/class/gpio/gpio%d/direction", io[i]);
		system(cmd);
	}

	while(1) {
		sleep(1);
		for(i=0; i<len; ++i) {
			memset(cmd, 0, sizeof(cmd));
			sprintf(cmd,"echo \"0\" > /sys/class/gpio/gpio%d/value",io[i]);
			system(cmd);
		}
		printf("ON\n");
		sleep(1);

		for(i=0; i<len; ++i) {
			memset(cmd, 0, sizeof(cmd));
			sprintf(cmd,"echo \"1\" > /sys/class/gpio/gpio%d/value",io[i]);
			system(cmd);
		}
		printf("OFF\n");
	}


	return EXIT_SUCCESS;
}
