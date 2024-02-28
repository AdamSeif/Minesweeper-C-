# Minesweeper Game

This is a simple implementation of the classic Minesweeper game using C++ and Qt framework.

## Introduction

This Minesweeper game features a grid-based interface where players can uncover cells to reveal numbers or mines underneath. The objective is to uncover all cells without detonating any mines.

## How to Play

- **Left Click**: Reveal a cell.
- **Right Click**: Flag or unflag a cell as a potential mine.

## Board Class (board.h)

### Description

The `Board` class is responsible for creating and managing the game board. It includes functionalities for initializing the board, setting mines, handling cell clicks, and managing game states.

### Public Members

- `Minesweeper(QWidget *parent = nullptr)`: Constructor to initialize the game window.
- `~Minesweeper()`: Destructor to clean up resources.
- `void lClick(int row, int col)`: Slot to handle left-click events on cells.
- `void rClick(int row, int col)`: Slot to handle right-click events on cells.

### Private Members

- `void prepareBoard()`: Initializes the game board layout.
- `void setMines()`: Randomly assigns mine locations on the board.
- `void expand(int row, int col)`: Expands the area when an empty cell is clicked.
- `void end(bool win)`: Ends the game with a win or loss condition.
- `void restart()`: Restarts the game.

### Constants

- `mines`: Total number of mines on the board.
- `ROWS`: Number of rows in the game board.
- `COLS`: Number of columns in the game board.

### Enumerations

- `State`: Represents the state of a cell (None, Flag, Question).

## MineButton Class

### Description

The `MineButton` class is a custom QPushButton subclass that handles mouse events, specifically right-click events.

### Public Members

- `MineButton(QWidget *parent = nullptr)`: Constructor to initialize the button.

### Protected Members

- `void mousePressEvent(QMouseEvent *event)`: Overrides the mousePressEvent to handle right-click events.

### Signals

- `void rClicked()`: Signal emitted when the button is right-clicked.

## Dependencies

- Qt framework

## Usage

To use this Minesweeper game, ensure you have the Qt framework installed and properly configured. You can then include the `board.h` header file in your project and create an instance of the `Minesweeper` class to start playing.

