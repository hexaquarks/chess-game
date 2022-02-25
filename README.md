# Chess Game Project
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)


Chess game app, made by [NicolasAlmerge](https://github.com/NicolasAlmerge) and [hexaquarks](https://github.com/hexaquarks).

## Installation 
 - Running the project requires [SFML](https://www.sfml-dev.org/download/sfml/2.5.1/) installed locally
 - Need **C++17** compiler installed locally
 
One can the compile the project from `[PATH]/chess_game/src/` by invoking `g++ -o ../main *.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio  -std=c++17 && ../main`
## Features
 - [x] Fully functional chess game, with respect to chess rules (*pin, check, checkmate, en passant, promotion, castle, etc*).
 - [x] Move history and move list traversal through <kbd> ← </kbd> <kbd> → </kbd> <kbd> ↑ </kbd> <kbd> ↓ </kbd>
 - [x] Dynamic square highlighting
 - [x] Smooth piece transition effect
 - [x] Functional menu options (*reset, flip board <kbd> Control </kbd>+<kbd> F </kbd>*)
 - [x] Mechanical utilities (*drag-and-drop, click-and-drop, right-click to reset piece, select/unselect, etc*)

## Demonstration

<div align="center" markdown="1">
<table>
    <thead>
        <tr>
            <td align="center"><code>Game Play Example</code></td>
            <td align="center"><code>Move List Traversal</code></td>
            <th align="center"><code>Square Highlighting</code></th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td align="center">
              <img src="https://github.com/hexaquarks/chess_game/blob/main/promotion/ChessGamePromotionGameplay.gif" title="Gameplay example" width="350"/>
          </td>
            <td align="center">
              <img src="https://github.com/hexaquarks/chess_game/blob/main/promotion/ChessGamePromotionTraversal.gif" title="Gameplay example" width="350"/>
          </td>
           <th align="center">
             <img src="https://github.com/hexaquarks/chess_game/blob/main/promotion/ChessGamePromotionHighlight.gif" title="Gameplay example" width="350"/>
          </th>
        </tr>
    </tbody>
</table>
</div>

## TODO List
  - Implement [Stockfish](https://github.com/official-stockfish/Stockfish) in the application
  - Implement [pgn](https://en.wikipedia.org/wiki/Portable_Game_Notation) file parser and reader
  - Ability to record user-generated chess lines on the board, to be saved in on a local file system
  - Use the pgn user-generated files to implement an *opening trainer*, similar to [puzzle rush](https://www.chess.com/puzzles/rush) and [puzzle storm](https://lichess.org/storm)
