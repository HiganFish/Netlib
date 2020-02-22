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
    EventHandler(Reactor *reactor, int fd, int option, int event_type,
                 void (*event_callback)(int fd, int option, int event_type, void *));

    void (*event_callback_)(int fd, int option, int event_type, void* args){};

    int fd_{};

    Reactor *reactor_;

    // 操作
    int option_;
    // 注册的事件
    int event_type_;

};

}
#endif //NETLIB_EVENTHANDLER_H
