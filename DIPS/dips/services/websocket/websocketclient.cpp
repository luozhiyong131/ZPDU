#include "websocketclient.h"
#include "common.h"
#include "pdu/dev/devSetting/devsetting.h"
#include "services/setpduthread.h"


WebSocketClient::WebSocketClient(QObject *parent) : QObject(parent)
{
    isConnected = false;
    mSocket = new QWebSocket();
    readFile();

    QObject::connect(mSocket, SIGNAL(connected()), this, SLOT(connected()));
    QObject::connect(mSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    QObject::connect(mSocket, SIGNAL(textMessageReceived(const QString &message)), this, SLOT(textMessageReceived(const QString &message)));

    SetPduThread::bulid();  //初始化
}

void WebSocketClient::open(const QUrl &url)
{
    if(!url.isEmpty())
        mSocket->open(url);
}

void WebSocketClient::readFile()
{
    QString url = sys_configFile_readStr("url", WEB_SOCKET);
    if(url.isEmpty()) {
        url = "localhost";
        sys_configFile_writeParam("url", "localhost", WEB_SOCKET);
    }

    QString port = sys_configFile_readStr("port", WEB_SOCKET);
    if(port.isEmpty()) {
        port = "1234";
        sys_configFile_writeParam("port", "1234", WEB_SOCKET);
    }
    m_url = "ws://" + url +":" + port;
    open(m_url);
}

bool WebSocketClient::sendMessage(const QJsonObject &message)
{
    bool ret = false;

    if(isConnected) {
        QJsonDocument doc(message);
        int rtn = mSocket->sendTextMessage(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
        if(rtn > 0)  ret = true;
    } else {
        open(m_url);
    }

    return ret;
}

void WebSocketClient::connected()
{
    isConnected = true;
}

void WebSocketClient::disconnected()
{
    isConnected = false;
}

void WebSocketClient::textMessageReceived(const QString &message)
{
    SetPduThread::bulid()->addList(message);
}

