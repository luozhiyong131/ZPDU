#include <QMutex>
#include <QAtomicInt>

#include <set>
#include <map>
#include <atomic>

#include "peersafe/imapi/im.h"
#include "zebra_delegates.h"
#include "utils.h"

//global objs
#ifdef ZEBRA_MODULE
peersafe::im::Message_client *_im = new peersafe::im::Message_client();
zebra_delegates* im_dele = new zebra_delegates;
#else
peersafe::im::Message_client *_im = nullptr;
zebra_delegates* im_dele = nullptr;
#endif
peersafe::im::Message_client &im = *_im;

std::set<std::string> g_dedup_set;
std::map<std::string, friend_info> friend_map;
QMutex friend_map_lock;


std::atomic_bool zebra_acquire_flag{false};
std::atomic_int zebra_network_state{0};
