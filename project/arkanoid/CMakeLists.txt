set(SOURCRS
    main.cpp
)
add_executable(arkanoid ${SOURCRS})
target_link_libraries(arkanoid net_lib)