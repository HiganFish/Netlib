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

Net::Reactor *preactor;

void SpliceEach(int sendfd, int recvfd)
{
    splice(sendfd, nullptr, pipfd[1], nullptr, 32768, SPLICE_F_MORE);
    splice(pipfd[0], nullptr, recvfd, nullptr, 32768, SPLICE_F_MORE);
//
//    char* buffer[1024]{};
//    int ret = recv(sendfd, buffer, sizeof(buffer), 0);
//    auto* buff = (float*)buffer;
//
//    float result = 0;
//    for (int i = 0; i < ret/ sizeof(float*); ++i)
//    {
//        result+=buff[i];
//    }
//    printf("%f\n", result);
//
//    send(recvfd, buffer, sizeof(buffer), 0);
}

void read_cb(int fd, int option, int event_type, void *user_data)
{
    LOG_INFO("send each a msg");
    SpliceEach(fd, fd);
}

void listen_cb(int fd, sockaddr_in *addr, size_t addrlen, void *user_data)
{
    auto *handler = new Net::EventHandler(preactor, fd, EPOLL_CTL_ADD, EPOLLIN | EPOLLRDHUP | EPOLLET, read_cb);
    preactor->AddEventHandler(handler);
}

int main()
{
    pipe(pipfd);

    Net::Log::SetLogger(Net::OUT_CONSOLE, Net::LOG_LEVEL_INFO);

    Net::Reactor reactor{};
    preactor = &reactor;

    int port = 8001;

    Net::Listener listener{&reactor, listen_cb, port};

    reactor.ReactorDispatch();
}