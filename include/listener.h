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
    Listener(Reactor *reactor, void (*listener_cb)(int fd, char *ip_buffer, const int &port, void* args), const int &port);

    static void (*cb)(int fd, char *ip_buffer, const int &port, void* args);

    static void ListenerCallBack(EventHandler *handler, void *args);

    int fd_;

private:
    Listener(const Listener&);
    Listener& operator=(const Listener&);
};

}
#endif //NETLIB_LISTENER_H
