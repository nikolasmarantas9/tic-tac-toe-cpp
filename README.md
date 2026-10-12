# Tic-Tac-Toe (Τρίλιζα) in C++

A classic, console-based **Tic-Tac-Toe** game implemented in C++ using Object-Oriented/Modular programming principles. Designed with clean input validation, dynamic board rendering, and support for replayability.

## Features
- **Two-Player Gameplay:** Take turns between Player `X` and Player `O`.
- **Input Validation:** Prevents crashes or illegal moves if invalid characters or already taken cells are chosen.
- **Automatic Win & Draw Detection:** Instantly checks horizontal, vertical, and diagonal winning patterns.
- **Replay Option:** Allows users to play multiple rounds without restarting the program.

## How to Compile and Run

1. Make sure you have a C++ compiler installed (like `g++` via MinGW or Visual Studio).
2. Open your terminal in the project directory.
3. Compile the code:
   ```bash
   g++ main.cpp -o tictactoe