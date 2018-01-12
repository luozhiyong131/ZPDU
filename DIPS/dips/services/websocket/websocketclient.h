#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QObject>
#include <QtWebSockets/QtWebSockets>

#define WEB_SOCKET  "websocket"

class WebSocketClient : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketClient(QObject *parent = nullptr);

    bool sendMessage(const QJsonObject &message);

signals:
    void closed();

protected:
    void readFile();
    void open(const QUrl &url);

protected slots:    
    void connected();
    void disconnected();
    void textMessageReceived(const QString &message);

private:
    QWebSocket *mSocket;
    bool isConnected;
    QUrl m_url;
};

#endif // WEBSOCKETCLIENT_H
