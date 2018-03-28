#ifndef UTILS_H
#define UTILS_H

#include <iostream>

struct friend_info
{
    friend_info(const std::string &channel_name_, int channel_state_)
    {
        channel_name = channel_name_;
        channel_state = channel_state_;
    }

    std::string channel_name;
    int         channel_state;
};

#endif // UTILS_H
