//
// Created by lsmg on 3/13/20.
//

#include <sys/socket.h>
#include <csignal>
#include <unistd.h>
#include "network/eventhandler.h"
#include "network/opsignal.h"

int Net::OpSignal::signal_writefd = -1;
Net::Reactor *Net::OpSignal::reactor_ = nullptr;

bool Net::OpSignal::Add(int fd, int option, int event_type)
{
    SetSigEventHandler(fd, SigEventHandler);
}

bool Net::OpSignal::Del(int fd, int option, int event_type)
{
    return OpBase::Del(fd, option, event_type);
}

bool Net::OpSignal::Dispatch(int time)
{
    return OpBase::Dispatch(time);
}

void Net::OpSignal::Dealloc()
{
    OpBase::Dealloc();
}

Net::OpSignal::OpSignal(Reactor *reactor)
{
    reactor_ = reactor;
    socketpair(AF_UNIX, SOCK_STREAM, 0, pipefd);
    signal_writefd = pipefd[1];

    // pipefd[1] 信号回调函数将信号写入pipefd[1] 然后就能够从 pipefd[0]读出
    event_ = new EventHandler(reactor_, pipefd[0], EventHandler::EventType::EV_READ, SignalCb);
    reactor_->AddEventHandler(event_);
}

Net::OpSignal::~OpSignal()
{

}

void Net::OpSignal::SigEventHandler(int sig)
{
    char msg = sig;
    write(signal_writefd, &msg, 1);
}

int Net::OpSignal::SetSigEventHandler(int signal, void (*handler)(int))
{
    struct sigaction sa{};
    sa.sa_handler = handler;
    sa.sa_flags |= SA_RESTART;
    sigfillset(&sa.sa_mask);

    if (sigaction(signal, &sa, nullptr) == -1)
    {
        // error
    }
}

void Net::OpSignal::SignalCb(Net::EventHandler *handler, void *args)
{
    char buffer[1024]{};
    int n = recv(handler->GetFd(), buffer, sizeof(buffer), 0);
    for (int i = 0; i < n; ++i)
    {
        EventHandler *handler =  reactor_->GetHandlerBySignal(buffer[i]);

        handler->event_callback_(handler, &buffer[i]);
    }
}
