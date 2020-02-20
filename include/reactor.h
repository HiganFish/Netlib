//
// Created by lsmg on 2/20/20.
//

#ifndef NETLIB_REACTOR_H
#define NETLIB_REACTOR_H


#include <bits/types/struct_timeval.h>
#include "opbase.h"
#include "eventhandler.h"

namespace Net
{
class Reactor
{
public:
    Reactor();

    OpBase* op_base_;

    void AddEventHandler(EventHandler* event_handler);

    int ReactorDispatch();

private:
    int reactor_break_later_;

    bool reactor_break_;

    bool reactor_continue_;

    bool running_loop;

    int ReactorLoop(int flags);

    int EventProcess();
};
}
#endif //NETLIB_REACTOR_H
