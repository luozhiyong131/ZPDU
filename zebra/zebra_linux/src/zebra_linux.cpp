//============================================================================
// Name        : zebra_linux.cpp
// Author      : Lzy
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "ZebraUdpThread.h"

int main()
{
	zebra_udp_thread();
	while(1) {
		cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
		sleep(1);
	}
	return 0; //
}
