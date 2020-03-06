//
// Created by lsmg on 2/20/20.
//

#ifndef NETLIB_EVENTHANDLER_H
#define NETLIB_EVENTHANDLER_H

#include "reactor.h"

namespace Net
{

// Reactor模式 EventHandler
class EventHandler
{
public:
    static const int BUFFER_SIZE = 1024;
    static const int IP_SIZE = 16;

    void (*event_callback_)(EventHandler* handler, void* args);

    EventHandler(Reactor *reactor, int fd, int option, int event_type,
                 void (*event_callback)(EventHandler *handler, void *args));

    Reactor *GetReactor() const;

    void SetReactor(Reactor *reactor);

    int GetFd() const;

    void SetFd(int fd);

    int GetOption() const;

    void SetOption(int option);

    int GetEventType() const;

    void SetEventType(int eventType);

    char *GetBuffer() const;

    const char *GetIp() const;

    void SetIp(const char *ip);

    int GetPort() const;

    void SetPort(int port);
private:
    Reactor *reactor_;

    int fd_;
    // 操作
    int option_;

    // 注册的事件
    int event_type_;

    // 通用缓存
    char *buffer;
    const char *ip_;
    int port_;

    EventHandler(const EventHandler&);
    EventHandler& operator=(const EventHandler&);
};

}
#endif //NETLIB_EVENTHANDLER_H
