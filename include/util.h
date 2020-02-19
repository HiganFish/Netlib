//
// Created by lsmg on 2/19/20.
//

#ifndef NETLIB_UTIL_H
#define NETLIB_UTIL_H

#include <cstring>
#include <cerrno>
#include <cstdio>

#define exit_if(r, ...) \
{   \
if (r)  \
{   \
    printf(__VA_ARGS__);    \
    printf("FILE: %s, LINE: %d --- errno: %d, errormsg %s",    \
            __FILE__, __LINE__, errno, strerror(errno));    \
}   \
}   \

#endif //NETLIB_UTIL_H
