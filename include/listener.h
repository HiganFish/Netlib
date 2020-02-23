//
// Created by lsmg on 2/21/20.
//

#ifndef NETLIB_LISTENER_H
#define NETLIB_LISTENER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include "reactor.h"

namespace Net
{

class Listener
{
public:
    Listener(Reactor *reactor, void (*listener_cb)(int fd, sockaddr_in *addr, socklen_t addrlen, void* user_data), const int &port);

    static void (*cb)(int fd, sockaddr_in *addr, socklen_t addrlen, void* user_data);

    static void ListenerReadCallBack(int fd, int option, int event_type, void *p);

    int fd_;

};

}
#endif //NETLIB_LISTENER_H
