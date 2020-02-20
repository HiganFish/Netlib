//
// Created by lsmg on 2/20/20.
//

#ifndef NETLIB_OPEPOLL_H
#define NETLIB_OPEPOLL_H

#include "opbase.h"

namespace Net
{
class OpEpoll : OpBase
{
    void *Init(Reactor reactor) override;

    int Add(Reactor reactor, EventHandler *event_handler) override;

    int Del(Reactor reactor, EventHandler *event_handler) override;

    int Dispatch(Reactor reactor, timeval *time) override;

    void Dealloc(Reactor reactor) override;
};
}
#endif //NETLIB_OPEPOLL_H
