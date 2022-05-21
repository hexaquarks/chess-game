cmake_minimum_required(VERSION 3.16.3)

set(This BoardReset)

set(Sources
    BoardReset.cpp
)

add_executable(${This} ${Sources})
target_link_libraries(${This} PUBLIC
    gtest_main
    chess_game
    sfml-graphics
    sfml-window
    sfml-system
)

add_test(
    NAME ${This}
    COMMAND ${This}
)