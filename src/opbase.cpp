//
// Created by lsmg on 2/20/20.
//

#include "opbase.h"

void *Net::OpBase::Init(Net::Reactor reactor)
{
    return nullptr;
}

int Net::OpBase::Add(Net::Reactor reactor, Net::EventHandler *event_handler)
{
    return 0;
}

int Net::OpBase::Del(Net::Reactor reactor, Net::EventHandler *event_handler)
{
    return 0;
}


int Net::OpBase::Dispatch(Net::Reactor reactor, timeval *time)
{
    return 0;
}

void Net::OpBase::Dealloc(Net::Reactor reactor)
{

}

