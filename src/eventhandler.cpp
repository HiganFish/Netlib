//
// Created by lsmg on 2/20/20.
//

#include "eventhandler.h"


Net::EventHandler::EventHandler(Net::EventHandler *event_handler, Net::Reactor *reactor, int fd, short event_type,
                                void *(*event_callback)(int, short, void *))
{
    event_handler->reactor_ = reactor;
    event_handler->fd_ = fd;
    event_handler->event_type_ = event_type;
    event_handler->event_callback_ = event_callback;

}
