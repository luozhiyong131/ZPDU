/*
 * cfg_op.h
 *
 *  Created on: 2019年8月8日
 *      Author: Lzy
 */

#ifndef CFG_OP_H_
#define CFG_OP_H_


//获取配置项
int cfg_item_read(const char *pFileName /*in*/, const char *pKey /*in*/, char * pValue/*in out*/, int * pValueLen /*out*/);

//写配置项
int cfg_item_write(const char *pFileName /*in*/, const char *pItemName /*in*/, const char *pItemValue/*in*/);


#endif /* CFG_OP_H_ */
