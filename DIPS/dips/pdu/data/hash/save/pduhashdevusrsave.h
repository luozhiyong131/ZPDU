#ifndef PDUHASHDEVUSRSAVE_H
#define PDUHASHDEVUSRSAVE_H
#include "pduhashcom.h"

typedef enum {
    PDU_CMD_USR_NAME=1, //用户名、密码
    PDU_CMD_USR_EMAIL,  // 用户邮件
    PDU_CMD_USR_PHONE, // 用户手机
    PDU_CMD_USR_GROUP, // 用户组
    PDU_CMD_USR_DEL, // 删除一个用户
    PDU_CMD_USR_ClEAR, // 删除所有用户
}PDU_CMD_DEV_USR_;

// 用户组相关
typedef enum {


}PDU_CMD_USR_GROUP_;


// 输出位权限相关
typedef enum {


}PDU_CMD_USR_OUTPUT_;

typedef enum {
    PDU_CMD_DEVUSR = 6, //设备信息 主功能码为6

    PDU_CMD_USR_INFO=1,
    PDU_CMD_USRGROUPP,
    PDU_CMD_USROUTPUT,

}PDU_CMD_DEV_USR;

void pdu_hashDevUsr_save(PduUsrManager *usr,pdu_dev_data *data);

#endif // PDUHASHDEVUSRSAVE_H
