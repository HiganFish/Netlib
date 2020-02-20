//
// Created by lsmg on 2/20/20.
//

#include <opepoll.h>
#include "reactor.h"

Net::Reactor::Reactor()
{
    reactor_break_ = false;
    reactor_break_later_ = false;
    reactor_continue_ = false;


    op_base_ = new OpEpoll();
    op_base_->Init(this);
}

void Net::Reactor::AddEventHandler(Net::EventHandler *event_handler)
{
    op_base_->Add(this, event_handler);
}

int Net::Reactor::ReactorDispatch()
{
    return ReactorLoop(0);
}

int Net::Reactor::ReactorLoop(int flags)
{
    // 一个Reactor 只能有一个循环
    if (running_loop)
    {
        return -1;
    }

    running_loop = true;

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

        int ret = op_base_->Dispatch(this, -1);
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
    return 0;
}
