#ifndef PDUHASHOUTPUTSAVE_H
#define PDUHASHOUTPUTSAVE_H
#include "pduhashcom.h"

// 输出位信息，主功能码表
typedef enum {
    PDU_CMD_OUTPUT_NAME = 10, // 输出位名称    
    PDU_CMD_OUTPUT_GROUP, //输出位分组信息
    PDU_CMD_OUTPUT_CUT,   // 输出位超限断电
    PDU_CMD_OUTPUT_SW_CTRL,// 输出位开关控制
    PDU_CMD_OUTPUT_TIME, // 输出位上下电时间
    PDU_CMD_OUTPUT_SORT, //输出位序号

    PDU_CMD_GROUP_CUT, // 组管理 超限断电
    PDU_CMD_GROUP_SW_CTRL, // 组管理 控制开关
    PDU_CMD_ALL_SW_CTRL, // 输出位，全开、全断
    PDU_CMD_GROUP_TIME, // 组管理，输出位上下电时间

}PDU_CMD_OUTPUT_BIT;

void pdu_output_name(PduOutputName *name, pdu_dev_data *data);
void pdu_output_sort(PduOutputSort *dev, pdu_dev_data *data);

void pdu_output_group(PduGroupsInfo *groupInfo, pdu_dev_data *data);
void pdu_output_cutOff(PduCutOff *cut, pdu_dev_data *data);
void pdu_group_cutOff(PduCutOff *cut, pdu_dev_data *data);

void pdu_output_SWctrl(PduSWControl *ctrl, pdu_dev_data *data);
void pdu_group_SWctrl(PduSWControl *ctrl, pdu_dev_data *data);
void pdu_all_SWctrl(PduSWControl *ctrl, pdu_dev_data *data);

void pdu_output_time(PduTimeSwitch *timeSW, pdu_dev_data *data);
void pdu_group_time(PduTimeSwitch *timeSW, pdu_dev_data *data);

#endif // PDUHASHOUTPUTSAVE_H
