//
// Created by lsmg on 2/19/20.
//

#ifndef NETLIB_UTIL_H
#define NETLIB_UTIL_H

#include <string>
#include <cstdlib>

namespace Net
{
int GetPortFromArg(int &argc, char **argv);

void ReplaceAll(std::string &str, const std::string &from, const std::string &to);
}

#endif //NETLIB_UTIL_H
