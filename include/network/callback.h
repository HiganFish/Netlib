//
// Created by lsmg on 2/23/20.
//

#ifndef NETLIB_CALLBACK_H
#define NETLIB_CALLBACK_H

class EventHandler;
namespace Net
{
    static int single_pipe[2]{-1, -1};

    void ZeroCopyCallback(int from_fd, int to_fd);
}
#endif //NETLIB_CALLBACK_H
