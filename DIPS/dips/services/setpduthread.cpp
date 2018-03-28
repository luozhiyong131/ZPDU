/****************************************************************************
**
** Copyright (C) 2018-1-31 By_MW
**
** 这个文件是用来将json转换为对应的PDU操作指令，并对其操作的线程文件。
**
****************************************************************************/

/*!
    \class SetPduThread

    \inmodule services

    \brief

    \internal
*/

#include <QDebug>
#include <QFile>

#include "setpduthread.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "pdu/dev/devSetting/devsetting.h"

SetPduThread::SetPduThread(QObject *parent)
{
    version1 = new Version1(this);
    start(); //开启线程
}

SetPduThread::~SetPduThread()
{
    isRun = false;
}

SetPduThread *SetPduThread::bulid()
{
    static SetPduThread* sington = nullptr;
    if(sington == nullptr)
        sington = new SetPduThread();
    return sington;
}

void SetPduThread::addList(const QString &message)
{
    QWriteLocker locker(&lock);
    m_memoryList.append(message);
}

// QString >> QJson
static QJsonObject getJsonObjectFromString(const QString jsonString)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toLocal8Bit().data());
    if( jsonDocument.isNull() ){
        qDebug()<< "===> please check the string "<< jsonString.toLocal8Bit().data();
    }
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject;
}

// QJson >> QString
static QString getStringFromJsonObject(const QJsonObject& jsonObject)
{
    return QString(QJsonDocument(jsonObject).toJson());
}

/**
 * @brief 打开json文件，读取array  -- 测试用
 * @param array
 */
static void readFile(QByteArray &array)
{
    QFile file("test.json");
    if(file.open(QIODevice::ReadOnly ))
    {
        qDebug() <<"文件打开成功！";
        QTextStream in(&file);
        array = in.readAll().toLatin1();
        file.close();
    }
    else
        qDebug() << "文件打开失败！";
}

void SetPduThread::run()
{
    isRun = true;
    while(isRun) {
        //----------[测试++]-----------
        QByteArray array;
        readFile(array);
        addList(QString(array));

        //----------------------------
        if(m_memoryList.size() > 0){
            lock.lockForRead(); //读锁
            QJsonObject  json = getJsonObjectFromString(m_memoryList.at(0));
            lock.unlock();
            lock.lockForWrite(); //写锁
            m_memoryList.removeAt(0);
            lock.unlock();

            if(QString::compare(json.value("company").toString(), "CLEVER")) {
                qDebug() << json.value("company").toString() << "company not is CLEVER";
                return;
            }
            versionControl(json);  //设置操作
        }
        sleep(1);
        //sleep(mSec);
    }
}

void SetPduThread::versionControl(QJsonObject &json)
{
    //公司 -- 版本
   // qDebug() << json.value("company").toString()<< json.value("version").toInt();
    int version = json.value("version").toInt();
    switch (version) {
    case 1: version1->versionV1_0(json); break; /* V1.0版本 */
    case 2:
        break;
    default:
        break;
    }
}

