set(SOURCRS
    main.cpp
        game.cpp game.h gameroom.cpp gameroom.h player.cpp player.h)
add_executable(arkanoid ${SOURCRS})
target_link_libraries(arkanoid net_lib)