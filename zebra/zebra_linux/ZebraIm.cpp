/*
 * ZebraIm.cpp
 *
 *  Created on: 2019年8月9日
 *      Author: Lzy
 */

#include "ZebraIm.h"

//
//Zebra_Im::Zebra_Im(Zebra_Client *c) : im(c)
//{
//    isRun = false;
//}
//
//Zebra_Im::~Zebra_Im()
//{
//    stopZebra();
//}
//
//Zebra_Im *Zebra_Im::bulid()
//{
//    static Zebra_Im* sington = NULL;
//    static Zebra_Client *c = NULL;
//    if(sington == NULL) {
//        c = new Zebra_Client;
//        sington = new Zebra_Im(c);
//    }
//
//    return sington;
//}
//
//bool Zebra_Im::startZebra()
//{
//    if(!isRun) {
//        start();
//        isRun = true;
//    }
//
//    return isRun;
//}
//
//
//void Zebra_Im::stopZebra()
//{
//    if(isRun) {
//        stop();
//        isRun = false;
//    }
//}
//
//
//int Zebra_Im::send(const string &id, uchar *buf, int len)
//{
//    int ret = 0;
//    if(isRun) {
//        string data((char*)buf,len);
//        ret = send_message(id, data);
//    }
//
//    return ret;
//}

