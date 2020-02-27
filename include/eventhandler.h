//
// Created by lsmg on 2/20/20.
//

#ifndef NETLIB_EVENTHANDLER_H
#define NETLIB_EVENTHANDLER_H

#include "reactor.h"

namespace Net
{

class EventHandler
{
public:
    static const int BUFFER_SIZE = 1024;
    static const int IP_SIZE = 16;

    Reactor *reactor_;

    int fd_;

    // 操作
    int option_;
    // 注册的事件
    int event_type_;

    // 通用缓存
    char buffer[BUFFER_SIZE];

    char ip_[IP_SIZE];
    int port_;

    EventHandler(Reactor *reactor, int fd, int option, int event_type,
                 void (*event_callback)(EventHandler *handler, void *args));

    void (*event_callback_)(EventHandler* handler, void* args){};

    void SetIpAndPort(char *ip, const int &port);

private:
    EventHandler(const EventHandler&);
    EventHandler& operator=(const EventHandler&);
};

}
#endif //NETLIB_EVENTHANDLER_H
