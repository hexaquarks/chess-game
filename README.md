# Chess Game Project
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## Installation
 - Running the project requires [SFML](https://www.sfml-dev.org/download/sfml/2.5.1/) installed locally
 - Need **C++17** compiler installed locally
One can then compile the project by running `make app`, and run the project by running `make run`.

## Long-term Goals
The long-term goal for this project is to have a puzzle trainer similar [Puzzle Rush](https://www.chess.com/puzzles/rush) and [Puzzle Storm](https://lichess.org/storm) but for openings! The opening puzzles would be generated according to move variations that were manually entered by the user and saved in their user configuration as FEN or PGN files. 
> Chess Base 17 has this feature but at ~300$ it's inaccessible for most, hence the open sourcing project.

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
        </tr>
        <tr>
            <td align="center" colspan="2"><code>Move Tree Display</code></td>
        </tr>
        <tr>
            <td align="center" colspan="2">
              <img src="https://github.com/hexaquarks/chess_game/blob/main/promotion/ChessGamePromotionMoveTree.gif" title="Gameplay example" width="750"/>
            </td>
        </tr>
    </tbody>
</table>
</div>
