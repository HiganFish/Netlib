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

int pipfd[2];

int fd_array[2];
int fd_sub = 0;

Net::Reactor *preactor;

void SpliceEach(int sendfd, int recvfd)
{
    splice(sendfd, nullptr, pipfd[1], nullptr, 32768, SPLICE_F_MORE);
    splice(pipfd[0], nullptr, recvfd, nullptr, 32768, SPLICE_F_MORE);
}

void read_cb(int fd, int option, int event_type, void *user_data)
{
    LOG_INFO("recv a msg");
    if(fd_sub  == 2)
    {
        LOG_INFO("send each a msg");
        SpliceEach(fd, fd_array[0] == fd ? fd_array[1]:fd_array[0]);
    }
}

void listen_cb(int fd, sockaddr_in *addr, size_t addrlen, void *user_data)
{
    auto *handler = new Net::EventHandler(preactor, fd, EPOLL_CTL_ADD, EPOLLIN | EPOLLRDHUP | EPOLLET, read_cb);
    preactor->AddEventHandler(handler);

    if (fd_sub <= 1)
    {
        fd_array[fd_sub++] = fd;
    }
}

int main()
{
    pipe(pipfd);

    Net::Log::SetLogger(Net::OUT_CONSOLE, Net::LOG_LEVEL_INFO);

    Net::Reactor reactor{};
    preactor = &reactor;

    int port = 8000;

    Net::Listener listener{&reactor, listen_cb, port};

    reactor.ReactorDispatch();
}