//
// Created by lsmg on 2/20/20.
//

#include <sys/epoll.h>
#include <util.h>
#include <log.h>
#include "opepoll.h"

bool Net::OpEpoll::Add(int fd, int option, int event_type)
{
    SetNonblocking(fd);

    epoll_event ev{};
    ev.events = event_type;
    ev.data.fd = fd;
    int ret = epoll_ctl(epfd_, option, fd, &ev);
    ERROR_IF(ret == -1, "epoll_ctl add a new fd:%d", fd)

    return ret != -1;

}

bool Net::OpEpoll::Del(int fd, int option, int event_type)
{
    return OpBase::Del(0, 0, 0);
}

bool Net::OpEpoll::Dispatch(int time)
{
    int ret = epoll_wait(epfd_, events_, EPOLL_EVENT_INIT_SIZE, time);
    ERROR_IF(ret == -1, "epoll_wait return %d", ret)

    for (int i = 0; i < ret; ++i)
    {
        int fd = events_[i].data.fd;
        int what = events_[i].events;
         switch (what)
         {
             case EPOLLIN:
                 reactor_->io_queue->push({fd, IO_READ});
                 break;
             case EPOLLOUT:
                 reactor_->io_queue->push({fd, IO_WRITE});
                 break;
             case EPOLLRDHUP:
                 reactor_->io_queue->push({fd, IO_CLOSE});
                 break;
             default:
                 LOG_WARN("unknow event type %d", what);
                 break;
         }
    }
}

void Net::OpEpoll::Dealloc()
{
    OpBase::Dealloc();
}

Net::OpEpoll::OpEpoll(Reactor *reactor)
{
    reactor_ = reactor;

    events_ = new epoll_event[EPOLL_EVENT_INIT_SIZE];

    epfd_ = epoll_create(EPOLL_EVENT_INIT_SIZE);
    SetNonblocking(epfd_);
}

void Net::OpEpoll::SetNonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL, 0);
    ERROR_IF(old_option == -1, "fcntl get fd:%d", fd);
    int ret = fcntl(fd, F_SETFL, old_option | O_NONBLOCK);
    ERROR_IF(ret == -1, "fcntl set fd:%d", fd);
}
