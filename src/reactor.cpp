//
// Created by lsmg on 2/20/20.
//

#include <opepoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <log.h>

#include "reactor.h"
#include "opbase.h"
#include "eventhandler.h"

Net::Reactor::Reactor()
{
    reactor_break_ = false;
    reactor_break_later_ = false;
    reactor_continue_ = false;

    running_loop_ = false;

    op_base_ = new OpEpoll(this);

    io_queue = new std::queue<io_event>;

    event_handler_array = new std::array<EventHandler*, 5000>;

    lisenfd_ = -1;
}

void Net::Reactor::AddEventHandler(Net::EventHandler *event_handler)
{
    (*event_handler_array)[event_handler->fd_] = event_handler;
    op_base_->Add(event_handler->fd_ , event_handler->option_, event_handler->event_type_);
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
                    LOG_DEBUG("a new read event fd:%d from %s:%d", fd, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
                }
                handler->event_callback_(handler, nullptr);
                break;
            case IO_WRITE:
                LOG_DEBUG("a new write event fd:%d from %s:%p", fd, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
                break;
            case IO_CLOSE:
                LOG_DEBUG("fd:%d from %s:%p closed", fd, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
                break;
        }
    }
    return 0;
}

void Net::Reactor::SetLisenerFd(int fd)
{
    lisenfd_ = fd;
}

