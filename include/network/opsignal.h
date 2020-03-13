//
// Created by lsmg on 3/13/20.
//
#ifndef NETLIB_OPSIGNAL_H
#define NETLIB_OPSIGNAL_H
#include "reactor.h"
#include "opbase.h"
namespace Net
{
class OpSignal : public OpBase
{

public:
    explicit OpSignal(Reactor *reactor);
    ~OpSignal() override;

    bool Add(int fd, int option, int event_type) override;

    bool Del(int fd, int option, int event_type) override;

    bool Dispatch(int time) override;

    void Dealloc() override;

    /**
     * 注册进入IO事件的回调函数， 用于调用信号相关的 用户设置的回调函数
     * @param handler
     * @param args
     */
    static void SignalCb(EventHandler *handler, void *args);
private:

    static Reactor *reactor_;
    EventHandler *event_;

    static int signal_writefd;
    int pipefd[2];

    static void SigEventHandler(int sig);

    int SetSigEventHandler(int signal, void (*handler)(int));
};
}

#endif //NETLIB_OPSIGNAL_H
