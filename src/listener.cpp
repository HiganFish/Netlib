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


void (*Net::Listener::cb)(int fd, char *ip_buffer, const int &port, void* args);

Net::Listener::Listener(Net::Reactor *reactor, void (*listener_cb)(int fd, char *ip_buffer, const int &port, void* args), const int &port)
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

    auto *event_handler = new EventHandler(reactor, fd_, EPOLL_CTL_ADD, EPOLLIN | EPOLLRDHUP, ListenerCallBack);

    // 注册事件控制器
    reactor->AddEventHandler(event_handler);
    reactor->SetLisenerFd(fd_);

}

void Net::Listener::ListenerCallBack(EventHandler *handler, void *args)
{

    sockaddr_in client_address{};
    socklen_t client_addresslen = sizeof(client_address);

    auto *addr = (sockaddr*)&client_address;
    int newfd = accept(handler->fd_, addr, &client_addresslen);

    if (newfd != -1)
    {
        char ip_buffer[64]{};
        inet_ntop(AF_INET, &client_address.sin_addr, ip_buffer, 64);
        int port = ntohs(client_address.sin_port);
        LOG_INFO("accept a new connection from %s:%d", ip_buffer, port);

        cb(newfd, ip_buffer, port, nullptr);
    }
}
