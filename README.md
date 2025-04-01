# 🎮 Tetris Game in C++  

A simple **Tetris game** built using **C++** with real-time console rendering. Control falling Tetrominoes, clear rows, and achieve the highest score!  

---

## 🌟 Table of Contents
- Project Description
- Key Features
- Installation
- Gameplay Controls
- How To Play
- Data Structure Analysis
- Object Oriented Structure
- Key Member Functions
- Code Structure
- Prerequisites 

---

## Features  

- **Classic Tetris Gameplay :** Move, rotate, and drop Tetrominoes to clear rows and score points.  

- **Dynamic Tetromino Movement :** Blocks can be moved left, right, rotated, and dropped instantly.  

- **7 Different Tetromino Shapes :** Includes I, O, T, S, Z, J, and L shapes.  

- **Gravity Mechanism :** Tetrominoes automatically fall over time, increasing difficulty.  

- **Line Clearing System :** Full rows disappear, increasing score and making space for new blocks.  

- **High Score Tracking :** Keeps track of the highest score achieved during gameplay.  

- **Real-Time Console Rendering :** Efficiently updates the game board without screen flickering.  

- **Game Over Detection :** Ends the game when no more moves are possible.

- **Increasing Difficulty :** Speed of falling tetriminos increases as the player socres more points.

---

## Installation

To run the tetris game, follow these steps:
1.  **Clone the Repository:**
    ```bash
    $ git clone [https://github.com/yourusername/tetris-game-cpp.git](https://github.com/yourusername/tetris-game-cpp.git)
    $ cd tetris-game-cpp
    ```
2.  **Compile the Code (using g++):**
    ```bash
    $ g++ tetris_game.cpp -o tetris_game.exe
    ```
3.  **Run the Game:**
    ```bash
    $ ./tetris_game.exe
    ```
---
## Gameplay Controls
- **Arrow Keys :**
    - `↑`(Up Arrow) → Rotate Tetromino
    - `←`(Left Arrow) → Move Tetromino Left
    - `↓`(Down Arrow) → Soft Drop (Move Tetromino Down Faster)
    - `→`(Right Arrow) → Move Tetromino Right
- **WASD Keys (Alternative Controls) :**
    - `W`→ Rotate
    - `A`→ Move Left
    - `S`→ Soft Drop
    - `D`→ Move Right
- **Other Controls :**
    - `Spacebar`→ Hard Drop (Instantly places Tetromino at the lowest possible position)
    - `X`→ Exit the Game
    - `R`→ Restart the Game
---
## How To Play

- **Objective :** Arrange falling Tetrominoes to create full horizontal rows.
- **Clearing Lines :** When a row is completely filled, it disappears, and you gain points.
- **Scoring System :**
   - Single Line Clear → `100 Points`
   
- **Game Over :** If the Tetrominoes reach the top of the board, the game ends.
- **High Score :** Your best score is saved and displayed at the end of the game.\
---
## Data Structure Analysis

- **2D Grid (Array) :** Represents the game board, where Tetrominoes are placed.
- **Vector (Dynamic Arrays) :** Stores different Tetromino shapes.
- **Structs :** Used for handling Tetromino properties (`shape`,`x`,`y` coordinates).
- **Enum :** Defines Tetromino types and movement directions.
---

## Object-Oriented Structure

| Class/Sturct           | Responsibility                                                   |
|----------------|-----------------------------------------------------------------|
|`Tetromino`|Handles individual block shapes, movement, and rotation|

---
## Key Member Functions

- `canMove(dx, dy, shape)` → Checks if Tetromino can move in a given direction.
- `rotateTetromino()` → Rotates the current Tetromino if space allows.
- `placeTetromino()` → Locks the Tetromino in place when it reaches the bottom.

- `handleInput()` → Handles keyboard controls for movement and rotation.
- `clearLines()` → Detects and clears completed rows.
- `spawnTetromino()` → Spawns a new Tetromino when the previous one is placed.
- `runGameSession()` → The main game loop controlling Tetromino spawning, movement, and game state.

---

## 📁 Code Structure
📂 Tetris Game/  
│── 📄 README.md  
│── 📄 tetris_game.cpp  
│── 📄 gameplay_tetris.mp4

## Prerequisites
- A C++ compiler (e.g g++)
- Windows (modifications required for Linux)

🎉 Thanks for checking out this project! Hope you enjoy playing! 🚀
