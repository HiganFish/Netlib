//
// Created by lsmg on 2/19/20.
//

#ifndef NETLIB_UTIL_H
#define NETLIB_UTIL_H

#include <string>
#include <cstdlib>

namespace Net
{
/**
 * 通过程序收到的参数 自动获取port
 * @param argc main函数参数
 * @param argv main函数参数
 * @return port
 */
int GetPortFromArg(int &argc, char **argv);
}

#endif //NETLIB_UTIL_H
