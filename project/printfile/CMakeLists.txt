set(SOURCRS
        main.cpp
        )
add_executable(printfile ${SOURCRS})
target_link_libraries(printfile net_lib)