set(NETWORK_DIR ../../include/network)

set(NETWORK_HEADERS
        ${NETWORK_DIR}/eventhandler.h
        ${NETWORK_DIR}/util.h
        ${NETWORK_DIR}/reactor.h
        ${NETWORK_DIR}/opbase.h
        ${NETWORK_DIR}/opepoll.h
        ${NETWORK_DIR}/listener.h
        ${NETWORK_DIR}/log.h
        ${NETWORK_DIR}/callback.h
        ${NETWORK_DIR}/protocol.h
        ${NETWORK_DIR}/opsignal.h
)

set(NETWORK_SOURSES
        network/eventhandler.cpp
        network/util.cpp
        network/reactor.cpp
        network/opbase.cpp
        network/opepoll.cpp
        network/listener.cpp
        network/log.cpp
        network/callback.cpp
        network/protocol.cpp
        network/opsignal.cpp
        )

add_library(net_lib ${NETWORK_HEADERS} ${NETWORK_SOURSES})

set(GAME_DIR ../../include/game)

set(GAME_HEADERS
    ${GAME_DIR}/player.h
    ${GAME_DIR}/gameroom.h
    ${GAME_DIR}/game.h
)

set (GAME_SOURSES
        game/player.cpp
        game/gameroom.cpp
        game/game.cpp)

add_library(game_lib ${GAME_HEADERS} ${GAME_SOURSES})