//
// Created by lsmg on 2/23/20.
//

#ifndef NETLIB_CALLBACK_H
#define NETLIB_CALLBACK_H
namespace Net
{
    static int single_pipe[2]{-1, -1};

    void Echo_Cb(int fd, int option, int event_type, void *user_data);
}
#endif //NETLIB_CALLBACK_H
