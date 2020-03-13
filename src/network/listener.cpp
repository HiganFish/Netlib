//
// Created by lsmg on 2/21/20.
//
#include <sys/socket.h>
#include <network/util.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <network/log.h>
#include "network/listener.h"
#include "network/eventhandler.h"

Net::Reactor *Net::Listener::reactor_;
void (*Net::Listener::cb)(Reactor *reactor, int fd, const char *ip, const int &port, void* args);

Net::Listener::Listener(Reactor *reactor, void (*listener_cb)(Reactor *reactor, int fd, const char *ip, const int &port, void* args), const int &port)
{
    // 使用0 初始化addr 不用填充 address.addr 因为INADDR_ANY = 0x00000000;
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    // 创建fd
    listenfd_ = socket(PF_INET, SOCK_STREAM, 0);
    ERROR_IF(listenfd_ < 0, "socket create")

    setsockopt(listenfd_, SOL_SOCKET, SO_REUSEADDR , (void*)1, 1);

    int ret = bind(listenfd_, (sockaddr*)&addr, sizeof(addr));
    ERROR_IF(ret == -1, "socket bind");

    cb = listener_cb;
    reactor_ = reactor;

    listen(listenfd_, 5);

    auto event_handler = new EventHandler(reactor, listenfd_, Net::EventHandler::EventType::EV_READ, ListenerCallBack);

    // 注册事件控制器
    reactor->AddEventHandler(event_handler);
    reactor->SetLisenerFd(listenfd_);

}

void Net::Listener::ListenerCallBack(EventHandler *handler, void *args)
{
    sockaddr_in client_address{};
    socklen_t client_addresslen = sizeof(client_address);

    auto *addr = (sockaddr*)&client_address;
    int newfd = accept(handler->GetFd(), addr, &client_addresslen);

    if (newfd != -1)
    {
        char *ip = new char[64]{};
        inet_ntop(AF_INET, &client_address.sin_addr, ip, 64);
        int port = ntohs(client_address.sin_port);
        LOG_DEBUG("accept a new connection from %s:%d", ip, port);

        cb(reactor_, newfd, ip, port, nullptr);
    }
}
