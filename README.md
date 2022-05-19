# Chess Game Project
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)


Chess game app, made by [NicolasAlmerge](https://github.com/NicolasAlmerge) and [hexaquarks](https://github.com/hexaquarks).

## Installation
 - Running the project requires [SFML](https://www.sfml-dev.org/download/sfml/2.5.1/) installed locally
 - Need **C++17** compiler installed locally

One can then compile the project by running `make app`, and run the project by running `make run`.
## Features
 - [x] Fully functional chess game, with respect to chess rules (*pin, check, checkmate, en passant, promotion, castle, etc*).
 - [x] Move history and move list traversal through <kbd> ← </kbd> <kbd> → </kbd> <kbd> ↑ </kbd> <kbd> ↓ </kbd>
 - [x] Dynamic square highlighting
 - [x] Arrow drawing (*normal, diagonal, L-shaped*)
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
 - [ ] Main Functionalities
     - [x] Chess Board
     - [x] Linked List data structure for move traversal
         - [x] Merge with Smooth Piece Transition
     - [ ] N-ary Tree data structure for move traversal and variation building
         - [ ] Merge with Smooth Piece Transition
         - [x] Design the custom iterator
         - [ ] Merge with right Side Panel move representation and traversal
     - [ ] Right Side Panel
         - [x] Draw move list on the panel as performed on the board
         - [x] Traverse to move by clicking on the side panel's move sub boxes
         - [ ] Top menu bar options
         - [ ] Bottom menu bar with options *(promote variation, delete variation)* [requires N-ary tree DS to be functional]
         - [x] Popup move selection window
     - [ ] Left Side Panel
         - [ ] Represent file structure [requires file system and pgn parser]
     - [ ] Fix sound buffer
     - [ ] Modularize further `GameThread.cpp`, and clean up code over all
     - [ ] implement an *opening trainer*, similar to [puzzle rush](https://www.chess.com/puzzles/rush) and [puzzle storm](https://lichess.org/storm) [end goal]
 - [ ] Utilities
     - [ ] Implement [Stockfish](https://github.com/official-stockfish/Stockfish) in the application
     - [ ] Implement [pgn](https://en.wikipedia.org/wiki/Portable_Game_Notation) file parser and reader
     - [ ] Ability to record user-generated chess lines on the board, to be saved in on a local file system
