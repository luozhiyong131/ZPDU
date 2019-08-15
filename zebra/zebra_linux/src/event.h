#ifndef __H_ZEBRA_IM_EVENT_H__
#define __H_ZEBRA_IM_EVENT_H__

#include <string>
#include <vector>

namespace peersafe { namespace imapi {

enum eventCode {
    BOOTSTRAP = 1000,
    GETDHTNODES = 1001,
    CREATECHANNEL = 2000,
    LISTENCHANNEL = 2001,
    JOINCHANNEL = 2002,
    ALLOWCHANNEL = 2003,
    DISALLOWCHANNEL = 2004,
    DESTROYCHANNEL = 2005,
    CHAT = 2006,
    NOTIFY = 2007,
    REPLY = 2008,
    CONNECTION = 8000,			// connect peers' event
    STORE = 9000,				// store pair of key and value
    FETCH = 9001,				// get pair of key and value
    LOADLOCALCHANNEL = 9002,	// load local channels
    UNKOWNEVENT = -1
};

enum channelType {
    UNKOWN=-1,
    CH_JOIN=0,
    CH_CHAT=1
};

class event {
public:
    virtual ~event() {}
protected:
    event() {}
};

class bootstrapEvent : public event {
public:
    struct Keypair {
        std::string Publick;
        std::string Private;
    };

    bootstrapEvent(const std::string& node_id, Keypair key)
    : node_id_(node_id)
    , key_(key){

    }

    const std::string& node_id() const {
        return node_id_;
    }

    const bootstrapEvent::Keypair& key() const {
        return  key_;
    }

private:
    std::string node_id_;
    Keypair key_;
};

class connectEvent : public event {
public:
    connectEvent(int errorCode,
        const std::string& id,
        const std::string& peer)
    : error_code_(errorCode)
    , id_(id)
    , peer_(peer) {}

    int error_code() {
        return error_code_;
    }

    const std::string& id() const {
        return id_;
    }

    const std::string& peer() const {
        return peer_;
    }

private:
    int error_code_;
    std::string id_;
    std::string peer_;
};

class dhtNodeEvent : public event {
public:
    dhtNodeEvent() {}

    void add(const std::string& node) {
        nodes_.push_back(node);
    }

    int nodes(std::vector<std::string>& nodes) {
        nodes.clear();
        nodes.swap(nodes_);
        return nodes.size();
    }
private:
    std::vector<std::string> nodes_;
};

class kvEvnet : public event {
public:
    kvEvnet(const std::string& key)
    : key_(key){
    }

    const std::string& key() const {
        return key_;
    }

    void value(const std::string& v) {
        value_ = v;
    }

    const std::string& value() const {
        return value_;
    }
private:
    std::string key_;
    std::string value_;
};

class channel {
public:
    struct Keypair {
        std::string Publick;
        std::string Private;
    };

    struct Owner {
        std::string id;
        struct Keypair keys;
    };

    struct audience_type{
        std::string id;
        std::string pubkey;
        std::string privatekey;
    };

    channel(int type,
        long expire,
        const std::string& channel_id,
        const channel::Keypair& key,
        const channel::Owner& owner,
        const std::string& secret,
        const channel::audience_type& from,
        const channel::audience_type& to)
    : type_(type)
    , expire_(expire)
    , channel_id_(channel_id)
    , key_(key)
    , owner_(owner)
    , secret_(secret)
    , from_(from)
    , to_(to) {}

    const int type() const {
        return type_;
    }

    const long expire() const {
        return expire_;
    }

    const std::string& channel_id() const {
        return channel_id_;
    }

    const channel::Keypair& key() const {
        return  key_;
    }

    const channel::Owner& owner() const {
        return owner_;
    }

    const std::string& secret() const {
        return secret_;
    }

    const channel::audience_type& from() const {
        return from_;
    }

    const channel::audience_type& to() const {
        return to_;
    }

private:
    int type_;
    long expire_;
    std::string channel_id_;
    channel::Keypair key_;
    channel::Owner owner_;
    std::string secret_;
    audience_type from_;
    audience_type to_;
};

class channelEvent : public event {
public:
    channelEvent(int type,
        long expire,
        const std::string& channel_id,
        const channel::Keypair& key,
        const channel::Owner& owner,
        const std::string& secret,
        const channel::audience_type& from,
        const channel::audience_type& to)
    : channel_(type, expire, channel_id, key, owner, secret, from, to) {}

    const channel& getChannel() const {
        return channel_;
    }
private:
    channel channel_;
};

class localChannel : public event {
public:
    localChannel()
    : channels_() {}

    std::size_t add(const channel& ch) {
        channels_.push_back(ch);
        return channels_.size();
    }

    std::vector<channel>& channels() {
        return channels_;
    }
private:
    std::vector<channel> channels_;
};

class joinEvent : public event {
public:
    joinEvent()
    : allow_(false)
    , owner_id_()
    , channel_()
    , description_()
    , reason_() {
    }

    void allow(bool v) {
        allow_ = v;
    }

    bool allow() const {
        return allow_;
    }

    void owner_id(const std::string& owner) {
        owner_id_ = owner;
    }

    const std::string& owner_id() const {
        return owner_id_;
    }

    void channel(const std::string& ch) {
        channel_ = ch;
    }

    const std::string& channel() const {
        return channel_;
    }

    void description(const std::string& descr) {
        description_ = descr;
    }

    const std::string& description() const {
        return description_;
    }

    void reason(const std::string& r) {
        reason_ = r;
    }

    const std::string& reason() const {
        return reason_;
    }

private:
    bool allow_;					// 处理结果，0 表示允许加入， 否则禁止
    std::string owner_id_;
    std::string channel_;
    std::string description_;
    std::string reason_;			// 基本用于禁止加入的原因
};

class allowEvent : public event {
public:
    allowEvent()
    : join_channel_()
    , chat_channel_()
    , welcome_() {}

    void join_channel(const std::string& join_channel) {
        join_channel_ = join_channel;
    }

    const std::string& join_channel() const {
        return join_channel_;
    }

    void chat_channel(const std::string& chat_channel) {
        chat_channel_ = chat_channel;
    }

    const std::string& chat_channel() const {
        return chat_channel_;
    }

    void welcome(const std::string& w) {
        welcome_ = w;
    }

    const std::string& welcome() const {
        return welcome_;
    }
private:
    std::string join_channel_;
    std::string chat_channel_;
    std::string welcome_;
};

class disAllowEvent : public event {
public:
    disAllowEvent(const std::string& join_channel,
        const std::string& reason)
    : join_channel_(join_channel)
    , reason_(reason) {

    }

    const std::string& join_channel() const {
        return join_channel_;
    }

    const std::string& reason() const {
        return reason_;
    }

private:
    std::string join_channel_;
    std::string reason_;
};

class messageEvent : public event {
public:
    messageEvent(const std::string& channel,
        const std::string& from, const std::string& to,
        const std::string& data)
    : channel_(channel)
    , from_(from)
    , to_(to)
    , data_(data) {}

    const std::string& channel() const {
        return channel_;
    }

    const std::string& from() const {
        return from_;
    }

    const std::string& to() const {
        return to_;
    }

    const std::string& data() const {
        return data_;
    }
private:
    std::string channel_;
    std::string from_;
    std::string to_;
    std::string data_;
};

class notifyEvent : public event {
public:
    notifyEvent(const std::string& channel, int reason)
    : channel_(channel)
    , reason_(reason) {}

    const std::string& channel() const {
        return channel_;
    }

    const int reason() const {
        return reason_;
    }
private:
    std::string channel_;
    int reason_;
};

class replyEvent : public event {
public:
    replyEvent(int errorCode,
        unsigned int originatorType,
        const std::string& channel)
        : errorCode_(errorCode)
        , originatorType_(originatorType)
        , channel_(channel) {}

    int errorCode() const {
        return errorCode_;
    }

    unsigned int originatorType() const {
        return originatorType_;
    }

    const std::string& channel() const {
        return channel_;
    }

private:
    int errorCode_;
    unsigned int  originatorType_;
    std::string channel_;
};

}}

#endif // __H_ZEBRA_IM_EVENT_H__
