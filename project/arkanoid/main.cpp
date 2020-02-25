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

int pipfd[2];

Net::Reactor *preactor;

void ReadCallback(Net::EventHandler *handler, void *args)
{
    memset(handler->buffer, 0, sizeof(handler->buffer));
    int len = recv(handler->fd_, handler->buffer, BUFFER_SIZE, 0);
    if (handler->proto_operate.Decode((uint8_t*)handler->buffer, len))
    {
        while (!handler->proto_operate.IsEmpty())
        {
            Net::ProtoMsg *msg = handler->proto_operate.Front();
            handler->proto_operate.Pop();
        }
    }
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

    reactor.ReactorDispatch();
}