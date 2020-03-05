set(NETWORK ../../include/network)

set(PUBLIC_HEADERS
        ${NETWORK}/eventhandler.h
        ${NETWORK}/util.h
        ${NETWORK}/reactor.h
        ${NETWORK}/opbase.h
        ${NETWORK}/opepoll.h
        ${NETWORK}/listener.h
        ${NETWORK}/log.h
        ${NETWORK}/callback.h
        ${NETWORK}/protocol.h
)

set(netlib_sources
        network/eventhandler.cpp
        network/util.cpp
        network/reactor.cpp
        network/opbase.cpp
        network/opepoll.cpp
        network/listener.cpp
        network/log.cpp
        network/callback.cpp
        network/protocol.cpp
)

add_library(net_lib ${PUBLIC_HEADERS} ${netlib_sources})
