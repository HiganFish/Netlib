//
// Created by lsmg on 2/20/20.
//

#ifndef NETLIB_EVENTHANDLER_H
#define NETLIB_EVENTHANDLER_H

#include "reactor.h"
#include "protocol.h"

#define BUFFER_SIZE 1024
namespace Net
{
class EventHandler
{
public:
    EventHandler(Reactor *reactor, int fd, int option, int event_type,
                 void (*event_callback)(EventHandler *handler, void *args));

    void (*event_callback_)(EventHandler* handler, void* args){};

    Reactor *reactor_;

    int fd_;

    // 操作
    int option_;
    // 注册的事件
    int event_type_;

    // 对应的二进制协议解析
    ProtoOperate proto_operate;

    // 通用缓存
    char buffer[BUFFER_SIZE];
};

}
#endif //NETLIB_EVENTHANDLER_H
