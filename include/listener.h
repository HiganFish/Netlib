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
    Listener(Reactor *reactor, void (*listener_cb)(EventHandler *handler, void* user_data), const int &port);

    static void (*cb)(EventHandler *handler, void* args);

    static void ListenerCallBack(EventHandler *handler, void *args);

    int fd_;

private:
    Listener(const Listener&);
    Listener& operator=(const Listener&);
};

}
#endif //NETLIB_LISTENER_H
