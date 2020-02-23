//
// Created by lsmg on 2/23/20.
//

#ifndef NETLIB_HTTP_H
#define NETLIB_HTTP_H

#include <string>
namespace Net
{
class Http
{
public:
    static void SendHttpResponse(int fd, std::string body);

    static std::string header;
};
}
#endif //NETLIB_HTTP_H
