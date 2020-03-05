//
// Created by lsmg on 2/19/20.
//

#ifndef NETLIB_UTIL_H
#define NETLIB_UTIL_H

#include <string>
#include <cstdlib>

namespace Net
{
// 传入 main函数参数 可以获得 port 仅支持单参数port
int GetPortFromArg(int &argc, char **argv);
}

#endif //NETLIB_UTIL_H
