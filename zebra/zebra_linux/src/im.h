#ifndef __H_ZEBRA_IM_H__
#define __H_ZEBRA_IM_H__

#include <string>
#include "event.h"

//#include "peersafe/imapi/event.h"
//#include "peersafe/core/rate.h"

namespace peersafe { namespace imapi {

class client {
public:
    virtual ~client() {}

    virtual unsigned int defaultPort() = 0;
    virtual void node_id(std::string& id) = 0;
    virtual void key_pair(std::string& public_key, std::string& private_key) = 0;
    // read only mode
    virtual bool readOnly() const = 0;
    virtual bool disableSignature() const { return false; }
    // nodes eg: 10.100.0.100:37053;10.100.0.101:37053
    virtual std::size_t bootstraps(std::string& nodes) = 0;
    virtual std::size_t usingMaxMemory() = 0;
    virtual int client_threads() = 0;
    virtual int service_threads() = 0;

    // handle events
        virtual void onEvent(eventCode code, event* evn) = 0;
protected:
    client() {}
};

class im_impl;
class im {
public:
    im(client* c);

    void start();
    void stop();

    int createChannel(int type, long expire, const std::string& name, std::string& channel_id);
    int destroyChannel(const std::string& channel_id);
    int joinChannel(const std::string& channel_id, const std::string& description);
    int listenChannel(const std::string& channel_id);

    int send_message(const std::string& channel_id, const std::string& data);

    // post a event using for getting data
    // where client::onEvent would handle an event
    int postEvent(peersafe::imapi::eventCode event);
//    void enable_rate(peersafe::core::rate* rate);

private:
    im_impl *impl_;
};

}}

#endif // __H_ZEBRA_IM_H__
