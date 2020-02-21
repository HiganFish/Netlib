//
// Created by lsmg on 2/20/20.
//

#include <opepoll.h>

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
}

void Net::Reactor::AddEventHandler(Net::EventHandler *event_handler)
{
    event_handler_array[event_handler->fd_] = event_handler;
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
            break;
        }

        if (reactor_break_)
        {
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

        EventHandler *handler = event_handler_array[event.fd];
        int fd = event.fd;

        switch (event.event_type)
        {
            case IO_READ:
                handler->event_callback_(fd, 0, 0, nullptr);
                break;
            case IO_WRITE:
                break;
            case IO_CLOSE:
                break;
        }
    }
    return 0;
}

