//
// Created by lsmg on 2/20/20.
//

#include <network/opepoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <network/log.h>
#include <csignal>
#include <network/opsignal.h>

#include "network/reactor.h"
#include "network/opbase.h"
#include "network/eventhandler.h"

Net::Reactor::Reactor()
{
    reactor_break_ = false;
    reactor_break_later_ = false;
    reactor_continue_ = false;

    running_loop_ = false;

    io_queue = new std::queue<io_event>;

    event_handler_array = new std::array<EventHandler*, 5000>;
    signal_event_array = new std::array<EventHandler*, 100>;

    op_base_ = new OpEpoll(this);
    sig_base_ = new OpSignal(this);
    lisenfd_ = -1;
}

void Net::Reactor::AddEventHandler(Net::EventHandler *event_handler)
{
    EventHandler::EventType event_type = event_handler->GetEventType();
    if (event_type == EventHandler::EventType::EV_READ)
    {
        (*event_handler_array)[event_handler->GetFd()] = event_handler;
        op_base_->Add(event_handler->GetFd(), EPOLL_CTL_ADD, EPOLLIN | EPOLLRDHUP | EPOLLET);
    }

    if (event_type == EventHandler::EventType::EV_SIGNAL)
    {
        (*signal_event_array)[event_handler->GetFd()] = event_handler;
        sig_base_->Add(event_handler->GetFd(), 0, 0);
    }
}

int Net::Reactor::ReactorDispatch()
{
    return ReactorLoop(0);
}

int Net::Reactor::ReactorLoop(int flags)
{
    // 一个Reactor 只能有一个循环
    if (running_loop_)
    {
        return -1;
    }

    running_loop_ = true;

    bool done = false;
    while (!done)
    {
        if (reactor_break_later_)
        {
            LOG_DEBUG("loop break immediately");
            break;
        }

        if (reactor_break_)
        {
            LOG_DEBUG("loop break later");
            break;
        }

        int ret = op_base_->Dispatch(-1);
        if (ret < 0)
        {
            break;
        }
        int n = EventProcess();
    }

    return 0;
}

int Net::Reactor::EventProcess()
{
    while(!io_queue->empty())
    {
        io_event event = io_queue->front();
        io_queue->pop();

        EventHandler *handler = (*event_handler_array)[event.fd];
        int fd = event.fd;

        sockaddr_in addr{};
        socklen_t addr_len = sizeof(addr);

        if (fd != lisenfd_)
        {
            getpeername(fd, (sockaddr*)(&addr), &addr_len);
        }

        switch (event.event_type)
        {
            case IO_READ:
                if (fd != lisenfd_)
                {
                    LOG_DEBUG("a new read event_ fd:%d from %s:%d", fd, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
                }
                handler->SetTickType(IO_READ);
                handler->event_callback_(handler, nullptr);
                break;
            case IO_WRITE:
                LOG_DEBUG("a new write event_ fd:%d from %s:%p", fd, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
                break;
            case IO_CLOSE:
                LOG_DEBUG("fd:%d from %s:%p closed", fd, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
                handler->SetTickType(IO_CLOSE);
                handler->event_callback_(handler, nullptr);
                break;
        }
    }
    return 0;
}

void Net::Reactor::SetLisenerFd(int fd)
{
    lisenfd_ = fd;
}

Net::EventHandler *Net::Reactor::GetHandlerBySignal(int signal)
{
    return (*signal_event_array)[signal];
}

