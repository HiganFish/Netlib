//
// Created by lsmg on 2/19/20.
//
#include <reactor.h>
#include <eventhandler.h>
#include <listener.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <log.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <callback.h>

int pipfd[2];

Net::Reactor *preactor;

void LisenerCb(int fd, sockaddr_in *addr, socklen_t addrlen, void *user_data)
{
    auto *handler = new Net::EventHandler(preactor, fd, EPOLL_CTL_ADD, EPOLLIN | EPOLLRDHUP | EPOLLET, Net::Echo_Cb);
    preactor->AddEventHandler(handler);
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        exit(0);
    }
    pipe(pipfd);

    Net::Log::SetLogger(Net::OUT_CONSOLE, Net::LOG_LEVEL_INFO);

    Net::Reactor reactor{};
    preactor = &reactor;

    int port = atoi(argv[1]);

    Net::Listener listener{&reactor, LisenerCb, port};

    reactor.ReactorDispatch();
}