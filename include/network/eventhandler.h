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

    enum class EventType : short
    {
        EV_READ = 0x01,
        EV_WRITE = 0x02,
        EV_SIGNAL = 0x04,
        EV_ET = 0x08
    };

    void (*event_callback_)(EventHandler* handler, void* args);

    /**
     * 生成新的EventHandler
     * @param reactor 指定从属的Reactor
     * @param fd  IO事件对应文件描述符 信号事件对应信号值
     * @param event_type 事件类型使用EventType传参
     * @param event_callback 回调函数
     */
    EventHandler(Reactor *reactor, int fd, EventType event_type,
                 void (*event_callback)(EventHandler *handler, void *args));

    Reactor *GetReactor() const;

    void SetReactor(Reactor *reactor);

    int GetFd() const;

    void SetFd(int fd);

    EventType GetEventType() const;

    void SetEventType(EventType event_type);

    char *GetBuffer() const;

    const char *GetIp() const;

    void SetIp(const char *ip);

    int GetPort() const;

    void SetPort(int port);

    io_evnet_type GetTickType() const;

    void SetTickType(io_evnet_type tickType);

    static const int BUFFER_SIZE = 1024;

    EventHandler(const EventHandler&) = delete;
    EventHandler& operator=(const EventHandler&) = delete;
private:
    static const int IP_SIZE = 16;

    Reactor *reactor_;

    int fd_;

    // 注册的事件
    EventType event_type_;

    io_evnet_type tick_type_;

    // 通用缓存
    char *buffer;
    const char *ip_;
    int port_;

public:
    void SignalCallback(int signal);
};

}
#endif //NETLIB_EVENTHANDLER_H
