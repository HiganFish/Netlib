//
// Created by lsmg on 2/20/20.
//

#include <sys/epoll.h>
#include <fcntl.h>
#include "network/log.h"
#include "network/opepoll.h"

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
    if ((ret == 0) && (errno != EINTR))
    {
        LOG_WARN("epoll_wait failure");
    }

    for (int i = 0; i < ret; ++i)
    {
        int fd = events_[i].data.fd;
        int what = events_[i].events;

        if (what & EPOLLIN)
        {
            reactor_->io_queue->push({fd, IO_READ});
        }
        if (what & EPOLLOUT)
        {
            reactor_->io_queue->push({fd, IO_WRITE});
        }
        if (what & EPOLLRDHUP)
        {
            reactor_->io_queue->push({fd, IO_CLOSE});
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
    fcntl(fd, F_SETFL, old_option | O_NONBLOCK);
}
