//
// Created by lsmg on 2/19/20.
//
#include <reactor.h>
#include <eventhandler.h>
#include <listener.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void listen_cb(int fd, sockaddr_in *addr, size_t addrlen, void *user_data)
{
    printf("a new socket fd: %d from %s:%d accpeted\n", fd, inet_ntoa(addr->sin_addr), ntohs(addr->sin_port));
}

int main()
{
    Net::Reactor reactor{};

    int port = 8001;

    Net::Listener listener{&reactor, listen_cb, port};

    reactor.ReactorDispatch();
}