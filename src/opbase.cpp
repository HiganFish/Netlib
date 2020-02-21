//
// Created by lsmg on 2/20/20.
//

#include "opbase.h"

bool Net::OpBase::Add(int fd, int option, int event_type)
{
    return 0;
}

bool Net::OpBase::Del(int fd, int option, int event_type)
{
    return 0;
}


bool Net::OpBase::Dispatch(int time)
{
    return 1201;
}

void Net::OpBase::Dealloc()
{

}

Net::OpBase::OpBase()
{
    io_map_ = new std::map<int, int>;
}

