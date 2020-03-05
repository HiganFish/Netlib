//
// Created by lsmg on 2/23/20.
//

#include "network/util.h"
#include "network/log.h"

int Net::GetPortFromArg(int &argc, char **argv)
{
    if (argc != 2)
    {
        LOG_ERROR("wrong number of parameters");
        exit(0);
    }
    int port = atoi(argv[1]);
    if ((port < 8000) || (port > 10000))
    {
        LOG_ERROR("invaild parameters");
        exit(0);
    }
    LOG_DEBUG("the port is %d", port);
    return port;
}
