//
// Created by lsmg on 2/19/20.
//
#include <network/reactor.h>
#include <network/eventhandler.h>
#include <network/listener.h>
#include <network/callback.h>
#include <network/log.h>
#include <sys/epoll.h>
#include <network/util.h>

#include "samplegame.h"

SimpleGame *game;

void ReadCallback(Net::EventHandler *handler, void *args)
{
    memset(handler->GetBuffer(), 0, Net::EventHandler::BUFFER_SIZE);
    int len = recv(handler->GetFd(), handler->GetBuffer(), Net::EventHandler::BUFFER_SIZE, 0);

    game->Distribute(handler->GetFd(), handler->GetIp(), handler->GetFd(),len, (uint8_t*)handler->GetBuffer());

}

void LisenerCb(Net::Reactor *reactor, int fd, const char* ip, const int &port, void *args)
{
    auto *handler = new Net::EventHandler(reactor, fd, EPOLL_CTL_ADD, EPOLLIN | EPOLLRDHUP | EPOLLET,
                                          ReadCallback);

    handler->SetIp(ip);
    handler->SetPort(port);

    reactor->AddEventHandler(handler);
}

int main(int argc, char* argv[])
{
    Net::Log::SetLogger(Net::OUT_CONSOLE, Net::LOG_LEVEL_INFO);

    Net::Reactor reactor{};

    int port = Net::GetPortFromArg(argc, argv);

    Net::Listener listener{&reactor, LisenerCb, port};

    game = new SimpleGame(2);

    reactor.ReactorDispatch();
}