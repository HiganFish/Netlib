//
// Created by lsmg on 2/21/20.
//
#include <sys/socket.h>
#include <util.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <log.h>
#include "listener.h"
#include "eventhandler.h"


void (*Net::Listener::cb)(int fd, sockaddr_in *addr, size_t addrlen, void* user_data);

Net::Listener::Listener(Net::Reactor *reactor, void (*listener_cb)(int fd, sockaddr_in *addr, size_t addrlen, void* user_data), const int &port)
{

    // 使用0 初始化addr 不用填充 address.addr 因为INADDR_ANY = 0x00000000;
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    // 创建fd
    fd_ = socket(PF_INET, SOCK_STREAM, 0);
    ERROR_IF(fd_ < 0, "socket create")

    setsockopt(fd_, SOL_SOCKET, SO_KEEPALIVE, (void*)1, 1);

    int ret = bind(fd_, (sockaddr*)&addr, sizeof(addr));
    ERROR_IF(ret == -1, "socket bind");

    cb = listener_cb;

    listen(fd_, 64);

    auto *event_handler = new EventHandler(reactor, fd_, EPOLL_CTL_ADD, (int)EPOLLIN, ListenerReadCallBack);

    // 注册事件控制器
    reactor->AddEventHandler(event_handler);
    reactor->SetLisenerFd(fd_);

}

void Net::Listener::ListenerReadCallBack(int fd, int option, int event_type, void *p)
{

    sockaddr_in client_address{};
    socklen_t client_addresslen = sizeof(client_address);

    auto *addr = (sockaddr*)&client_address;
    int newfd = accept(fd, addr, &client_addresslen);
    ERROR_IF(newfd == -1, "accept a new connection from %s:%d", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port))

    cb(newfd, &client_address, client_addresslen, nullptr);
}
