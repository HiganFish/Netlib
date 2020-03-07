//
// Created by lsmg on 2/20/20.
//

#include <cstring>
#include "network/eventhandler.h"

Net::EventHandler::EventHandler(Net::Reactor *reactor, int fd, int option, int event_type,
                                void (*event_callback)(EventHandler *handler, void *))
                                :reactor_(reactor), fd_(fd), option_(option),event_type_(event_type),
                                event_callback_(event_callback), ip_(), port_(0), buffer()
{
    buffer = new char[BUFFER_SIZE]{};
    ip_ = new char[IP_SIZE]{};
}

Net::Reactor *Net::EventHandler::GetReactor() const
{
    return reactor_;
}

void Net::EventHandler::SetReactor(Net::Reactor *reactor)
{
    reactor_ = reactor;
}

int Net::EventHandler::GetFd() const
{
    return fd_;
}

void Net::EventHandler::SetFd(int fd)
{
    fd_ = fd;
}

int Net::EventHandler::GetOption() const
{
    return option_;
}

void Net::EventHandler::SetOption(int option)
{
    option_ = option;
}

int Net::EventHandler::GetEventType() const
{
    return event_type_;
}

void Net::EventHandler::SetEventType(int eventType)
{
    event_type_ = eventType;
}

char *Net::EventHandler::GetBuffer() const
{
    return buffer;
}

const char *Net::EventHandler::GetIp() const
{
    return ip_;
}

int Net::EventHandler::GetPort() const
{
    return port_;
}

void Net::EventHandler::SetPort(int port)
{
    port_ = port;
}

void Net::EventHandler::SetIp(const char *ip)
{
    ip_ = ip;
}

Net::io_evnet_type Net::EventHandler::GetTickType() const
{
    return tick_type_;
}

void Net::EventHandler::SetTickType(Net::io_evnet_type tickType)
{
    tick_type_ = tickType;
}
