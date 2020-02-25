//
// Created by lsmg on 2/23/20.
//

#include <fcntl.h>
#include <unistd.h>
#include <eventhandler.h>
#include <log.h>

#include "callback.h"

void Net::EchoCallback(EventHandler* handler, void *args)
{
    ZeroCopyCallback(handler->fd_, handler->fd_);
}

void Net::ZeroCopyCallback(int from_fd, int to_fd)
{
    if (single_pipe[0] == -1)
    {
        pipe(single_pipe);
    }

    splice(from_fd, nullptr, single_pipe[1], nullptr, 32768, SPLICE_F_MORE);
    splice(single_pipe[0], nullptr, to_fd, nullptr, 32768, SPLICE_F_MORE);

    LOG_INFO("from fd:%d to fd:%d Zero Copy", from_fd, to_fd);
}
