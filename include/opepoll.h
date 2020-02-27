//
// Created by lsmg on 2/20/20.
//

#ifndef NETLIB_OPEPOLL_H
#define NETLIB_OPEPOLL_H

#include <sys/epoll.h>
#include "reactor.h"
#include "opbase.h"

namespace Net
{

#define EPOLL_EVENT_INIT_SIZE 32

class OpEpoll : public OpBase
{
public:
    explicit OpEpoll(Reactor *reactor);

    bool Add(int fd, int option, int event_type) override;

    bool Del(int fd, int option, int event_type) override;

    bool Dispatch(int time_sec) override;

    void Dealloc() override;

private:
    Net::Reactor *reactor_;
    int epfd_;

    epoll_event *events_;

    static void SetNonblocking(int fd);

    OpEpoll(const OpEpoll&);
    OpEpoll& operator=(const OpEpoll&);
};
}
#endif //NETLIB_OPEPOLL_H
