/*
 * ZebraClient.h
 *
 *  Created on: 2019年8月9日
 *      Author: Lzy
 */

#ifndef ZEBRACLIENT_H_
#define ZEBRACLIENT_H_
#include "im.h"
#include "ZebraCfg.h"
#include <cstring>

using namespace peersafe;
using namespace imapi;

class Zebra_Client : public client
{
public:
	Zebra_Client();
	virtual ~Zebra_Client();

    void setBootstrapNodes(const string& nodes) { mBootstrapNodes = nodes;}
protected:
    uint defaultPort() {return 37051;}

    void node_id(string& id) {id = mNodeId;}
    void key_pair(string& public_key, string& private_key){}
    bool readOnly() const {return true;}

    // nodes eg: 10.100.0.100:37053;10.100.0.101:37053
    size_t bootstraps(string& nodes) {nodes = mBootstrapNodes; return nodes.length();}
    size_t usingMaxMemory() { return 512; }
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
    string mNodeId;
    string mBootstrapNodes;
    Zebra_Cfg *mCfg;
};

#endif /* ZEBRACLIENT_H_ */
