cmake_minimum_required(VERSION 3.16.3)

## Use CMake to download GoogleTest as part of the build's configure step, as opposed to git cloning manually
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
)
FetchContent_MakeAvailable(googletest)
target_link_libraries(gtest_main)

# Set the project name
project(chess_game)

# Set project specifications
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE) 

enable_testing()
set(Headers
    ./include/GameThread.hpp
)
set(Sources
    ./src/GameThread.cpp
)
# add the executable
add_library(chess_game STATIC ${Sources} ${Headers})
add_subdirectory(tests)