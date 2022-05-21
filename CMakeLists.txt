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
    ./include/Components/Board.hpp
    ./include/Components/MenuButton.hpp
    ./include/Components/MoveBox.hpp
    ./include/Components/MoveSelectionPanel.hpp
    ./include/Components/SidePanel.hpp
    ./include/Pieces/Bishop.hpp
    ./include/Pieces/King.hpp
    ./include/Pieces/Knight.hpp
    ./include/Pieces/Pawn.hpp
    ./include/Pieces/Piece.hpp
    ./include/Pieces/Queen.hpp
    ./include/Pieces/Rook.hpp
    ./include/Ressources/RessourceManager.hpp
    ./include/Utilities/Arrow.hpp
    ./include/Utilities/DrawableSf.hpp
    ./include/Utilities/Move.hpp
    ./include/Utilities/MoveList.hpp
    ./include/Utilities/MoveTree.hpp
    ./include/Utilities/MoveTreeNode.hpp
    ./include/Utilities/PieceTransition.hpp
    ./include/Utilities/StockfishConnector.hpp
)
set(Sources
    ./src/GameThread.cpp
    ./src/Components/Board.cpp
    ./src/Components/MenuButton.cpp
    ./src/Components/MoveBox.cpp
    ./src/Components/MoveSelectionPanel.cpp
    ./src/Components/SidePanel.cpp
    ./src/Pieces/Bishop.cpp
    ./src/Pieces/King.cpp
    ./src/Pieces/Knight.cpp
    ./src/Pieces/Pawn.cpp
    ./src/Pieces/Piece.cpp
    ./src/Pieces/Queen.cpp
    ./src/Pieces/Rook.cpp
    ./src/Ressources/RessourceManager.cpp
    ./src/Utilities/Arrow.cpp
    ./src/Utilities/DrawableSf.cpp
    ./src/Utilities/Move.cpp
    ./src/Utilities/MoveList.cpp
    ./src/Utilities/MoveTree.cpp
    ./src/Utilities/MoveTreeNode.cpp
    ./src/Utilities/PieceTransition.cpp
    ./src/Utilities/StockfishConnector.cpp
)
# add the executable
add_library(chess_game STATIC ${Sources} ${Headers})
add_subdirectory(tests)