//
// Created by lsmg on 2/20/20.
//

#include "opepoll.h"


void *Net::OpEpoll::Init(Net::Reactor reactor)
{
    return OpBase::Init(reactor);
}

int Net::OpEpoll::Add(Net::Reactor reactor, Net::EventHandler *event_handler)
{
    return OpBase::Add(reactor, event_handler);
}

int Net::OpEpoll::Del(Net::Reactor reactor, Net::EventHandler *event_handler)
{
    return OpBase::Del(reactor, event_handler);
}

int Net::OpEpoll::Dispatch(Net::Reactor reactor, timeval *time)
{
    return OpBase::Dispatch(reactor, time);
}

void Net::OpEpoll::Dealloc(Net::Reactor reactor)
{
    OpBase::Dealloc(reactor);
}
