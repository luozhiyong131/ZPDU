#include "web.h"
#include <QDebug>

WEB * WEB::mInstance = nullptr;

WEB::WEB(QObject *parent) : QObject(parent)
{
#ifdef ZEBRA_MODULE
    connect(SignalDispatch::get(), SIGNAL(update_zebra_security_list_sig(std::map<QString,zebra_security_data>)),
            this, SLOT(onUpdateZebraSecurityList(std::map<QString,zebra_security_data>)));
#endif
}

WEB *WEB::getInstance()
{
    if(mInstance == nullptr)
    {
        mInstance = new WEB();
    }
    return mInstance;
}

#ifdef ZEBRA_MODULE
void WEB::onUpdateZebraSecurityList(std::map<QString, zebra_security_data> mapData)
{
    std::map<QString, zebra_security_data>::iterator it;
    for(it = mapData.begin(); it != mapData.end(); it++)
    {
        if(it->second.beat_beriod > 0)
        {
            qDebug() << "[Web]_ON_" << it->second.dev_ip;
        }else{
            qDebug() << "[Web]_OFF_" << it->second.dev_ip;
        }
    }
}
#endif
