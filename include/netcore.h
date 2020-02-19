//
// Created by lsmg on 2/19/20.
//

#ifndef NETLIB_NETCORE_H
#define NETLIB_NETCORE_H
namespace Net{

class Socket
{
public:
    explicit Socket(int port);

    void InitTcp();
private:
    char* ip_;
    int port_;

    int listenfd_;
};
}
#endif //NETLIB_NETCORE_H
