//
// Created by lsmg on 2/23/20.
//

#include "util.h"
#include "log.h"

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

void Net::ReplaceAll(std::string &str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    if (str.empty())
    {
        return;
    }
    while((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
}