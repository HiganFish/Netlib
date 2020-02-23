set(NETLIB_INCLUDE_DIR ../include)

set(PUBLIC_HEADERS
        ${NETLIB_INCLUDE_DIR}/eventhandler.h
        ${NETLIB_INCLUDE_DIR}/util.h
        ${NETLIB_INCLUDE_DIR}/reactor.h
        ${NETLIB_INCLUDE_DIR}/opbase.h
        ${NETLIB_INCLUDE_DIR}/opepoll.h
        ${NETLIB_INCLUDE_DIR}/listener.h
        ${NETLIB_INCLUDE_DIR}/log.h
        ${NETLIB_INCLUDE_DIR}/callback.h
        ${NETLIB_INCLUDE_DIR}/http.h
)

set(netlib_sources
        eventhandler.cpp
        util.cpp
        reactor.cpp
        opbase.cpp
        opepoll.cpp
        listener.cpp
        log.cpp
        callback.cpp
        http.cpp
)

add_library(net_lib ${PUBLIC_HEADERS} ${netlib_sources})
