//
// Created by lsmg on 2/23/20.
//

#include <fcntl.h>
#include <unistd.h>
#include <network/eventhandler.h>
#include <network/log.h>

#include "network/callback.h"

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
