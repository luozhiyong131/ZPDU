/*
 * cfg_op.h
 *
 *  Created on: 2019年8月8日
 *      Author: Lzy
 */

#ifndef CFG_OP_H_
#define CFG_OP_H_

#ifndef __CFG_OP_H__
#define __CFG_OP_H__


#ifdef  __cplusplus
extern "C" {
#endif

//获取配置项
int GetCfgItem(char *pFileName /*in*/, char *pKey /*in*/, char * pValue/*in out*/, int * pValueLen /*out*/);

//写配置项
int WriteCfgItem(char *pFileName /*in*/, char *pItemName /*in*/, char *pItemValue/*in*/, int itemValueLen /*in*/);


#ifdef  __cplusplus
}
#endif



#endif



#endif /* CFG_OP_H_ */
