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
    EventHandler(EventHandler *event_handler, Reactor *reactor, int fd,
                 short event_type, void* (*event_callback)(int, short, void*));


private:

    void* (*event_callback_)(int, short, void*){};

    int fd_{};

    Reactor *reactor_{};

    // 注册的事件
    short event_type_{};

    // 被激活原因
    short event_res_{};

};

}
#endif //NETLIB_EVENTHANDLER_H
