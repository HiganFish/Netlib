//
// Created by lsmg on 2/20/20.
//

#include "eventhandler.h"

Net::EventHandler::EventHandler()
{

}

void Net::EventHandler::Init(Net::Reactor *reactor, int fd, int option, int event_type,
                             void (*event_callback)(int, int, int, void *))
{
    reactor_ = reactor;
    fd_ = fd;
    option_ = option;
    event_type_ = event_type;
    event_callback_ = event_callback;
}
