//
// Created by lsmg on 2/23/20.
//

#include <fcntl.h>
#include <unistd.h>

#include "callback.h"

void Net::Echo_Cb(int fd, int option, int event_type, void *user_data)
{
    if (single_pipe[0] == -1)
    {
        pipe(single_pipe);
    }

    splice(fd, nullptr, single_pipe[1], nullptr, 32768, SPLICE_F_MORE);
    splice(single_pipe[0], nullptr, fd, nullptr, 32768, SPLICE_F_MORE);
}
