cmake_minimum_required(VERSION 3.16.3)

set(This BoardTests)

set(Sources
    BoardReset.cpp
    PieceCapture.cpp
    EventHandling.cpp
)

add_executable(${This} ${Sources})
target_link_libraries(${This} PUBLIC
    gtest_main
    chess_game
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)

add_test(
    NAME ${This}
    COMMAND ${This}
)