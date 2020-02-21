//
// Created by lsmg on 2/21/20.
//
#include <sys/socket.h>
#include <util.h>
#include <netinet/in.h>
#include <sys/epoll.h>
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
    log_if(fd_ < 0, "socket failed!\n");

    setsockopt(fd_, SOL_SOCKET, SO_KEEPALIVE, (void*)1, 1);

    int ret = bind(fd_, (sockaddr*)&addr, sizeof(addr));
    log_if(ret == -1, "bind failed\n");

    cb = listener_cb;

    listen(fd_, 64);

    auto *event_handler = new EventHandler();

    event_handler->Init(reactor, fd_, EPOLL_CTL_ADD, (int)EPOLLIN, ListenerReadCallBack);

    // 注册事件控制器
    reactor->AddEventHandler(event_handler);

}

void Net::Listener::ListenerReadCallBack(int fd, int option, int event_type, void *p)
{

    sockaddr_in client_address{};
    socklen_t client_addresslen = sizeof(client_address);

    auto *addr = (sockaddr*)&client_address;
    int newfd = accept(fd, addr, &client_addresslen);

    cb(fd, &client_address, client_addresslen, nullptr);
}
