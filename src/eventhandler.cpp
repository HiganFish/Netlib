//
// Created by lsmg on 2/20/20.
//

#include "eventhandler.h"

#include <cstring>

Net::EventHandler::EventHandler(Net::Reactor *reactor, int fd, int option, int event_type,
                                void (*event_callback)(EventHandler *handler, void *))
                                :reactor_(reactor), fd_(fd), option_(option),event_type_(event_type),
                                event_callback_(event_callback), ip_(), port_(0), buffer()
{
}

void Net::EventHandler::SetIpAndPort(char *ip, const int &port)
{
    strcpy(ip_, ip);
    port_ = port;
}
