//
// Created by lsmg on 2/20/20.
//

#ifndef NETLIB_OPBASE_H
#define NETLIB_OPBASE_H

#include <bits/types/struct_timeval.h>
#include "reactor.h"

namespace Net {

enum op_feature
{
    FEATURE_ET = 0x01
};

class OpBase
{
public:
    virtual void* Init(Reactor reactor);

    virtual int Add(Reactor reactor, EventHandler* event_handler);

    virtual int Del(Reactor reactor, EventHandler* event_handler);

    virtual int Dispatch(Reactor reactor,timeval *time);

    virtual void Dealloc(Reactor reactor);

    op_feature op_features_;
};
}
#endif //NETLIB_OPBASE_H
