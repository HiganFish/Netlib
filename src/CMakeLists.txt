set(NETLIB_INCLUDE_DIR ../include)

set(PUBLIC_HEADERS
        ${NETLIB_INCLUDE_DIR}/netcore.h
        ${NETLIB_INCLUDE_DIR}/util.h
        ${NETLIB_INCLUDE_DIR}/netepoll.h
)

set(netlib_sources
        netcore.cpp
        netepoll.cpp
)

add_library(net_lib ${PUBLIC_HEADERS} ${netlib_sources})
