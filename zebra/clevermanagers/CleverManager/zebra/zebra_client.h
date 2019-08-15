#ifndef ZEBRA_CLIENT_H
#define ZEBRA_CLIENT_H
#include "im.h"
#include "zebra_datapacket.h"

using namespace peersafe;
using namespace imapi;

class Zebra_Client : public client
{
public:
    Zebra_Client();
    void setBootstrapNodes(const QString& nodes) { mBootstrapNodes = nodes;}

protected:
    uint defaultPort() {return 37050;}

    void node_id(std::string& id) {id = mNodeId;}
    void key_pair(std::string& public_key, std::string& private_key){}
    bool readOnly() const {return true;}

    // nodes eg: 10.100.0.100:37053;10.100.0.101:37053
    std::size_t  bootstraps(std::string& nodes) { nodes = mBootstrapNodes.toStdString();  return nodes.length(); }
    std::size_t  usingMaxMemory() { return 512; }
    int client_threads() { return 1; }
    int service_threads() { return 2; }

    void onEvent(eventCode code, event* evn);

private:
    void handleBootstrapEvent(bootstrapEvent *evn);
    void handleGetDHTNodesEvent(dhtNodeEvent *evn);
    void handleConnectPeerEvent(connectEvent* evn);
    void handleJoinChannelEvent(joinEvent* evn);
    void handleCreateChannelEvent(channelEvent* evn);
    void handleAllowEvent(allowEvent* evn);
    void handleDisAllowEvent(disAllowEvent* evn) ;
    void handleMessageEvent(messageEvent* evn);
    void handleReplyEvent(replyEvent* evn);
    void handleNotifyEvent(notifyEvent* evn);
    void handleLoadChEvent(localChannel* evn);

private:
    std::string mNodeId;
    QString mBootstrapNodes;
    Zebra_DataPacket *mZebraData;
};

#endif // ZEBRA_CLIENT_H
