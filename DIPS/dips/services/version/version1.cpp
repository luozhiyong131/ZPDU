#include "version1.h"

#define ISDEBUG 0

Version1::Version1(QObject *parent) : QObject(parent)
{
    vCommon = new VersionCommon(this);
}

void Version1::versionV1_0(QJsonObject &json)
{
    HeadInfo headInfo;
    //head_info（设备基本信息） 对象IP
    QJsonObject head_info = json.take("head_info").toObject();
    if(head_info.isEmpty()) {qDebug() << "head_info isEmpty"; return;}

    headInfo.ip  = head_info.value("ip").toString();   //ip
    headInfo.devNum = head_info.value("dev_num").toInt(); //设备号 0-9
    headInfo.devType = head_info.value("dev_type").toString();   //设备类型 ZPDU
    headInfo.devSpec = head_info.value("dev_spec").toInt();   //设备系统 ABCD

    //head_info(设备名字)
    setHeadInfoV1_0(head_info, headInfo);

    //line_item_list（设备输入相信息） 数组大小
    QJsonArray line_item_list = json.take("line_item_list").toArray();
    setlineV1_0(line_item_list, headInfo); //设置输入相

     //output_item_list （输出位信息）
    QJsonArray output_item_list = json.take("output_item_list").toArray();
    setOutPutV1_0(output_item_list, headInfo); //设置输出位

    if(json.contains("env_info")){ //环境项
        //env_info （环境项/子项信息列表）
        QJsonObject env_info = json.take("env_info").toObject();
            //hum_item_list (设备湿度信息)
            QJsonArray hum_item_list = env_info.take("hum_item_list").toArray();
            setHumV1_0(hum_item_list, headInfo); //设置湿度

            //tem_item_list （设备温度信息）
            QJsonArray tem_item_list = env_info.take("tem_item_list").toArray();
            setTemV1_0(tem_item_list, headInfo); //设置温度
    }

    //ip_addr （设备 IP 地址信息）
    QJsonObject ip_addr = json.take("ip_addr").toObject();
    setIpAddrV1_0(ip_addr, headInfo); //设置IP
}

void Version1::setHeadInfoV1_0(QJsonObject &json, HeadInfo &headInfo)
{
    if(json.isEmpty()) return;
    if(ISDEBUG) qDebug() << "head_info";
    net_dev_data pkt;
    //设备名字
    if(json.contains("dev_name")){
        //if(ISDEBUG) qDebug() << "dev_name is existence";
        pkt.addr = headInfo.devNum;
        pkt.fn[0] = 5; //设备信息
        pkt.fn[1] = 0x11; //名称
        QByteArray buf = json.value("dev_name").toString().toLatin1();
        pkt.data = (uchar *)buf.data();
        pkt.len = buf.length();
        sentData(pkt, headInfo);
    }
}

void Version1::setlineV1_0(QJsonArray &jsonArray, HeadInfo &headInfo)
{
    if(jsonArray.isEmpty())  return;
    int size = jsonArray.size();
    if(ISDEBUG) qDebug() << "line_size" << size;
    for(int i = 0; i < size; i++){
    QJsonObject json = jsonArray.at(i).toObject();
        net_dev_data pkt;
        int id = json.value("id").toInt();

        //电流
        if(!jsonIsEmpty(json, "cur")){
            //if(ISDEBUG) qDebug() << "cur is existence";
        pkt.addr = headInfo.devNum;
        pkt.fn[0] = 2; // 输入相电流
        pkt.fn[1] = id; //
        uchar data[10] ={0, 0,   0, 100,       0, 0,   0, 100};
        dataCompletion(json, "cur", data);
        pkt.data = data;
        pkt.len = 8;  //[8字节]
        sentData(pkt, headInfo);
        }

        //电压
        if(!jsonIsEmpty(json, "vol")){
            //if(ISDEBUG) qDebug() << "vol is existence";
        pkt.addr = headInfo.devNum;
        pkt.fn[0] = 1; // 输入相电压
        pkt.fn[1] = id; //
        uchar data[10] ={0, 0,   0, 100,   0, 0,   0, 100};
        dataCompletion(json, "vol", data);
        pkt.data = data;
        pkt.len = 8;  //[8字节]
        sentData(pkt, headInfo);
        }
    }
}

void Version1::setOutPutV1_0(QJsonArray &jsonArray, HeadInfo &headInfo)
{
    if(jsonArray.isEmpty())  return;
    int size = jsonArray.size();
    if(ISDEBUG) qDebug() << "output_size" << size;
    for(int i = 0; i < size; i++){
    QJsonObject json = jsonArray.at(i).toObject();
        net_dev_data pkt;
        int id = json.value("id").toInt();

        //电流
        if(!jsonIsEmpty(json, "cur")){
            //if(ISDEBUG) qDebug() << "cur is existence";
            pkt.addr = headInfo.devNum;
            pkt.fn[0] = 0; // 输出位电流
            pkt.fn[1] = id; //
            uchar data[10] ={0, 0,   0, 100,       0, 0,   0, 100};
            dataCompletion(json, "cur", data);
            pkt.data = data;
            pkt.len = 8;  //[2字节]
            sentData(pkt, headInfo);
        }

        //开关控制
        if(json.contains("switch")){
            //if(ISDEBUG) qDebug() << "switch is existence";
            pkt.addr = headInfo.devNum;
            pkt.fn[0] = 0x0D; // 输出位开关
            pkt.fn[1] = id; //
            uchar data[10] ={0, 0};
            data[0] = json.value("switch").toInt();
            pkt.data = data;
            pkt.len = 1;  //[1字节]
            sentData(pkt, headInfo);
        }

        //oup名字
        if(json.contains("name")){
            //if(ISDEBUG) qDebug() << "name is existence";
            pkt.addr = headInfo.devNum;
            pkt.fn[0] = 10; // 输出位名称
            pkt.fn[1] = id; //
            QByteArray buf = json.value("name").toString().toLatin1();
            pkt.data = (uchar *)buf.data();
            pkt.len = buf.length();
            sentData(pkt, headInfo);
        }
    }

}

void Version1::setHumV1_0(QJsonArray &jsonArray, HeadInfo &headInfo)
{
    if(jsonArray.isEmpty())  return;
    int size = jsonArray.size();
    if(ISDEBUG) qDebug() << "hum_size" << size;
    for(int i = 0; i < size; i++){
    QJsonObject json = jsonArray.at(i).toObject();
        net_dev_data pkt;
        int id = json.value("id").toInt();

        //湿度
        if(!jsonIsEmpty(json, "hum")){
            //if(ISDEBUG) qDebug() << "hum is existence";
        pkt.addr = headInfo.devNum;
        pkt.fn[0] = 4; // 湿度
        pkt.fn[1] = id; //
        uchar data[10] ={0, 0,   0, 100,       0, 0,   0, 100};
        dataCompletion(json, "hum", data);
        pkt.data = data;
        pkt.len = 8;  //[8字节]
        sentData(pkt, headInfo);
        }
    }
}

void Version1::setTemV1_0(QJsonArray &jsonArray, HeadInfo &headInfo)
{
    if(jsonArray.isEmpty())  return;
    int size = jsonArray.size();
    if(ISDEBUG) qDebug() << "tem_size" << size;
    for(int i = 0; i < size; i++){
    QJsonObject json = jsonArray.at(i).toObject();
        net_dev_data pkt;
        int id = json.value("id").toInt();

        //温度
        if(!jsonIsEmpty(json, "tem")){
            //if(ISDEBUG) qDebug() << "tem is existence";
        pkt.addr = headInfo.devNum;
        pkt.fn[0] = 4; // 温度
        pkt.fn[1] = id; //
        uchar data[10] ={0, 0,   0, 100,       0, 0,   0, 100};
        dataCompletion(json, "tem", data);
        pkt.data = data;
        pkt.len = 8;  //[2字节]
        sentData(pkt, headInfo);
        }
    }
}

void Version1::setIpAddrV1_0(QJsonObject &json, HeadInfo &headInfo)
{
    if(json.isEmpty()) return;
    if(ISDEBUG) qDebug() << "ip_addr";

    uchar fu = 0x10; // IP
    net_dev_data pkt;
    pkt.addr = 0;
    pkt.fn[0] = 7; // 设备网络信息

    //DHCP (0 不启用 1 启用)
    if(json.contains("mode")){
        if(ISDEBUG) qDebug() << "------------mode---------";
        pkt.fn[1] = fu + 6;
        uchar data[10] ={0, 0,};
        data[0] = json.value("mode").toInt();
        pkt.data = data;
        pkt.len = 1;  //[1字节]
        sentData(pkt, headInfo);
    }
    //ip (IP 地址)
    if(json.contains("ip")){
        if(ISDEBUG) qDebug() << "------------IP---------";
        QByteArray buf = json.value("ip").toString().toLatin1();
        pkt.fn[1] = fu + 1;
        pkt.data = (uchar *)buf.data();
        pkt.len = buf.length();
        sentData(pkt, headInfo);
    }
    //gw (默认网关)
    if(json.contains("gw")){
        if(ISDEBUG) qDebug() << "------------gw---------";
        QByteArray buf = json.value("gw").toString().toLatin1();
        pkt.fn[1] = fu + 2;
        pkt.data = (uchar *)buf.data();
        pkt.len = buf.length();
        sentData(pkt, headInfo);
    }
    //mask (子网掩码)
    if(json.contains("mask")){
        if(ISDEBUG) qDebug() << "------------mask---------";
        QByteArray buf = json.value("mask").toString().toLatin1();
        pkt.fn[1] = fu + 3;
        pkt.data = (uchar *)buf.data();
        pkt.len = buf.length();
        sentData(pkt, headInfo);
    }
    //dns (DNS 服务器)
    if(json.contains("dns")){
        if(ISDEBUG) qDebug() << "------------dns---------";
        QByteArray buf = json.value("dns").toString().toLatin1();
        pkt.fn[1] = fu + 4;
        pkt.data = (uchar *)buf.data();
        pkt.len = buf.length();
        sentData(pkt, headInfo);
    }
}

bool Version1::jsonIsEmpty(QJsonObject &json, QString name)
{
    QString value_min = name + "_min";
    QString value_max = name + "_max";
    QString value_critical_min = name + "_critical_min";
    QString value_critical_max = name + "_critical_max";

    return !(json.contains(value_min) && json.contains(value_max) &&
             json.contains(value_critical_min) && json.contains(value_critical_max));
}

void Version1::dataCompletion(QJsonObject &json, QString name, uchar *data)
{
    QString value_min = name + "_min";
    QString value_max = name + "_max";
    QString value_critical_min = name + "_critical_min";
    QString value_critical_max = name + "_critical_max";
    int value;
    value = json.value(value_min).toDouble() * 10;
    *data++ = value >> 8;
    *data++ = value;
    value = json.value(value_max).toDouble() * 10;
    *data++ = value >> 8;
    *data++ = value;
    value = json.value(value_critical_min).toDouble() * 10;
    *data++ = value >> 8;
    *data++ = value;
    value = json.value(value_critical_max).toDouble() * 10;
    *data++ = value >> 8;
    *data++ = value;
}

void Version1::sentData(net_dev_data &pkt, HeadInfo &headInfo)
{
    vCommon->sentData(pkt, headInfo);
}
