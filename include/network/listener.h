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
    Listener(Reactor *reactor, void (*listener_cb)(Reactor *reactor, int fd, const char *ip, const int &port, void* args),
            const int &port);

    static void (*cb)(Reactor *reactor, int fd, const char *ip, const int &port, void* args);

    static void ListenerCallBack(EventHandler *handler, void *args);

private:
    int listenfd_;
    static Reactor *reactor_;
    Listener(const Listener&);
    Listener& operator=(const Listener&);
};

}
#endif //NETLIB_LISTENER_H
