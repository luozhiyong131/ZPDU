#include "devmapfile.h"

#define DEV_CONFIG_FILE "devmap.json"

DevMapFile::DevMapFile()
{
    QString fn = cm_pathOfData(DEV_CONFIG_FILE);
    QFile file(fn);
    if(!file.exists()) {
        writeJson(fn);
    }
}


bool DevMapFile::writeJson(const QString &name)
{
    QJsonArray array;

    for(int i=0; i<20; ++i)
    {
        QJsonObject obj;
        QJsonObject addr;
        addr.insert("ip", "");
        addr.insert("devnum", -1);
        obj.insert(QString::number(i+1), addr);
        array.push_back(obj);
    }

    QJsonDocument jsonDoc(array);
    QByteArray ba = jsonDoc.toJson();
    QFile file(name);
    bool ret = file.open(QIODevice::WriteOnly);
    if(ret) {
        file.write(ba);
        file.close();
    } else {
        qDebug() << "write json file failed";
    }

    return ret;
}

bool DevMapFile::readJson(devRtuMap &map)
{
    QFile file(cm_pathOfData(DEV_CONFIG_FILE));
    bool ret = file.open(QIODevice::ReadOnly);
    if(ret)
    {
        QByteArray ba = file.readAll();
        QJsonParseError e;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(ba, &e);
        if(e.error == QJsonParseError::NoError && !jsonDoc.isNull())
        {
            QJsonArray jsonArray = jsonDoc.array();
            int size =  jsonArray.size();

            map.clear();
            for(int i=0; i<size; ++i) {
                QJsonObject obj = jsonArray.at(i).toObject();
                QString key = obj.keys().first();
                obj = obj.value(key).toObject();

                sMapItem item;
                item.ip = obj["ip"].toString();
                item.devNum = obj["devnum"].toInt();
                map.insert(key.toInt(), item);
            }
        } else {
            qDebug() << e.errorString().toUtf8().constData();// 检查错误类型
        }

    }  else {
        qDebug() << "read json file failed";
    }

    return ret;
}
