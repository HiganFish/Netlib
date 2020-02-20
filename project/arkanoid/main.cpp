//
// Created by lsmg on 2/19/20.
//
#include <reactor.h>
#include <eventhandler.h>

int main()
{
    Net::Reactor reactor{};

    reactor.ReactorDispatch();
}