#include "configfile.h"

#define CONFIG_FILE "config.json"


ConfigFile::ConfigFile()
{
    QString fn = cm_pathOfData(CONFIG_FILE);
    QFile file(fn);
    if(file.exists()) {
        readJson(fn);
    } else {
        writeJson(fn);
    }
}


bool ConfigFile::writeJson(const QString &name)
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

bool ConfigFile::readJson(const QString &name)
{
    QFile file(name);
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

            for(int i=0; i<size; ++i) {
                QJsonObject obj = jsonArray.at(i).toObject();
                QString key = obj.keys().first();
                obj = obj.value(key).toObject();

                qDebug() <<  key << obj["ip"].toString() << obj["devnum"].toInt();
            }
        } else {
            qDebug() << e.errorString().toUtf8().constData();// 检查错误类型
        }

    }  else {
        qDebug() << "read json file failed";
    }


    return ret;
}
