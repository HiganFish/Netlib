set(SOURCRS
    main.cpp
        samplegame.cpp samplegame.h)
add_executable(arkanoid ${SOURCRS})
target_link_libraries(arkanoid net_lib game_lib)