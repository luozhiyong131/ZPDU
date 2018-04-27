#include "versioncommon.h"
#include "pdu/dev/devSetting/devsetting.h"

VersionCommon::VersionCommon(QObject *parent) : QObject(parent)
{

}

void VersionCommon::sentData(net_dev_data &pkt, HeadInfo &headInfo)
{
    if(QString::compare(headInfo.ip.right(3), "255")){ //单独设置
        DevSetting::bulid()->sentData(headInfo.ip, pkt, headInfo.devType);
    }else{ //单独设置
        DevSetting::bulid()->sentData(pkt, headInfo.devType); //全局设置（仅适用于C类网络）
    }
}
