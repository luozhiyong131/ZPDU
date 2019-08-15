/*
 * ZebraChannel.h
 *
 *  Created on: 2019年8月12日
 *      Author: Lzy
 */

#ifndef SRC_ZEBRACHANNEL_H_
#define SRC_ZEBRACHANNEL_H_
#include "ZebraIm.h"

class Zebra_Channel
{
public:
	Zebra_Channel();
	virtual ~Zebra_Channel();
	void createChannel();

protected:
	bool checkChannel();
};

#endif /* SRC_ZEBRACHANNEL_H_ */
