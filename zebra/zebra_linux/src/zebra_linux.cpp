//============================================================================
// Name        : zebra_linux.cpp
// Author      : Lzy
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "ZebraPeer.h"

int main()
{
	Zebra_Peer *peer = Zebra_Peer::bulid();
	int i=0; char buf[64] = {0};

	while(1) {
		sprintf(buf, "Hello World %d", i++);
		peer->send((uchar *)buf, strlen(buf));

//		cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
		sleep(5);
	}
	return 0; //
}
