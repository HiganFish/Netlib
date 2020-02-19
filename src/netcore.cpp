//
// Created by lsmg on 2/19/20.
//

#include "netcore.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include "util.h"

Net::Socket::Socket(int port):port_(port)
{

}

void Net::Socket::InitTcp()
{
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(port_);
    address.sin_addr.s_addr = htons(INADDR_ANY);

    listenfd_ = socket(PF_INET, SOCK_STREAM, 0);
    exit_if(listenfd_ < 0, "socket failed");

    int ret = bind(listenfd_, (sockaddr*)&address, sizeof(address));
    exit_if(ret == -1, "bind failed");

    ret = listen(listenfd_, 5);
    exit_if(ret == -1, "listen failed");
}