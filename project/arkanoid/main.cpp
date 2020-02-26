//
// Created by lsmg on 2/19/20.
//
#include <reactor.h>
#include <eventhandler.h>
#include <listener.h>
#include <log.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <util.h>
#include "game.h"

int pipfd[2];

Net::Reactor *preactor;

Game *game;

void ReadCallback(Net::EventHandler *handler, void *args)
{
    memset(handler->buffer, 0, sizeof(handler->buffer));
    int len = recv(handler->fd_, handler->buffer, BUFFER_SIZE, 0);

    game->Distribute(handler->fd_, handler->ip_, handler->port_,
                     len, (uint8_t*)handler->buffer);
}

void LisenerCb(Net::EventHandler *handler, void *args)
{
    auto *phandler = new Net::EventHandler(preactor, handler->fd_, EPOLL_CTL_ADD, EPOLLIN | EPOLLRDHUP | EPOLLET,
                                          ReadCallback);

    preactor->AddEventHandler(phandler);
}

int main(int argc, char* argv[])
{
    pipe(pipfd);

    Net::Log::SetLogger(Net::OUT_CONSOLE, Net::LOG_LEVEL_INFO);

    Net::Reactor reactor{};
    preactor = &reactor;

    int port = Net::GetPortFromArg(argc, argv);

    Net::Listener listener{&reactor, LisenerCb, port};

    game = new Game();

    reactor.ReactorDispatch();
}