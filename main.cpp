
#include <iostream>
#include <array>
#include <string>
#include <algorithm>
#include <cctype>
#include <random>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

constexpr int BOARD_SIZE = 9;
using Board = array<char, BOARD_SIZE>;

const string RESET = "\033[0m";
const string RED = "\033[31m";
const string BLUE = "\033[34m";
const string GREEN = "\033[32m";
const string GOLD = "\033[33m";
const string CYAN = "\033[36m";
const string BOLD = "\033[1m";

const array<array<int, 3>, 8> WIN_PATTERNS = {{
    {{0, 1, 2}}, {{3, 4, 5}}, {{6, 7, 8}},
    {{0, 3, 6}}, {{1, 4, 7}}, {{2, 5, 8}},
    {{0, 4, 8}}, {{2, 4, 6}}
}};

struct Statistics {
    int xWins = 0;
    int oWins = 0;
    int draws = 0;
    int computerWins = 0;
    int computerLosses = 0;
    int computerDraws = 0;
};

enum class GameMode {
    TwoPlayers,
    EasyComputer,
    HardComputer
};

mt19937 rng(random_device{}());

// Ενεργοποίηση ANSI χρωμάτων στα Windows
void enableColors() {
#ifdef _WIN32
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    if (output != INVALID_HANDLE_VALUE) {
        DWORD mode = 0;
        if (GetConsoleMode(output, &mode)) {
            SetConsoleMode(output, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
    }
#endif
}

void clearScreen() {
    cout << "\033[2J\033[H";
}

void displayHeader() {
    cout << CYAN << BOLD;
    cout << "========================================\n";
    cout << "             TIC-TAC-TOE                \n";
    cout << "                TRILIZA                 \n";
    cout << "========================================\n";
    cout << RESET;
}

Board createBoard() {
    Board board{};
    for (int i = 0; i < BOARD_SIZE; ++i) {
        board[i] = static_cast<char>('1' + i);
    }
    return board;
}

void displayBoard(const Board& board) {
    cout << "\n";

    for (int row = 0; row < 3; ++row) {
        cout << "                 ";

        for (int col = 0; col < 3; ++col) {
            int index = row * 3 + col;
            char cell = board[index];

            if (cell == 'X') {
                cout << RED << BOLD << " X " << RESET;
            } else if (cell == 'O') {
                cout << BLUE << BOLD << " O " << RESET;
            } else {
                cout << GOLD << " " << cell << " " << RESET;
            }

            if (col < 2) cout << "|";
        }

        cout << "\n";

        if (row < 2) {
            cout << "                ---+---+---\n";
        }
    }

    cout << "\n";
}

bool checkWin(const Board& board, char player) {
    for (const auto& pattern : WIN_PATTERNS) {
        if (board[pattern[0]] == player &&
            board[pattern[1]] == player &&
            board[pattern[2]] == player) {
            return true;
        }
    }
    return false;
}

bool checkDraw(const Board& board) {
    return all_of(board.begin(), board.end(), [](char cell) {
        return cell == 'X' || cell == 'O';
    });
}

bool isValidMove(const Board& board, int choice) {
    return choice >= 1 &&
           choice <= BOARD_SIZE &&
           board[choice - 1] != 'X' &&
           board[choice - 1] != 'O';
}

int getNumber(const string& prompt, int minimum, int maximum) {
    string input;

    while (true) {
        cout << prompt;

        if (!(cin >> input)) {
            return -1;
        }

        bool digitsOnly = !input.empty() &&
            all_of(input.begin(), input.end(), [](unsigned char ch) {
                return isdigit(ch);
            });

        if (digitsOnly) {
            try {
                int number = stoi(input);
                if (number >= minimum && number <= maximum) {
                    return number;
                }
            } catch (const exception&) {
                // Η είσοδος δεν χωρά σε int.
            }
        }

        cout << RED << "Invalid input. Enter a number from "
             << minimum << " to " << maximum << ".\n"
             << RESET;
    }
}

bool askYesNo(const string& prompt) {
    string input;

    while (true) {
        cout << prompt;

        if (!(cin >> input)) return false;

        if (input.size() == 1) {
            char answer = static_cast<char>(
                tolower(static_cast<unsigned char>(input[0]))
            );

            if (answer == 'y') return true;
            if (answer == 'n') return false;
        }

        cout << GOLD << "Please enter y or n.\n" << RESET;
    }
}

int getPlayerMove(const Board& board, char player) {
    while (true) {
        int choice = getNumber(
            string(" Player ") + player +
            ", choose a position (1-9): ", 1, 9
        );

        if (choice == -1) return -1;

        if (isValidMove(board, choice)) {
            return choice;
        }

        cout << RED << "That position is already occupied. Try again.\n"
             << RESET;
    }
}

int getRandomMove(const Board& board) {
    array<int, BOARD_SIZE> available{};
    int count = 0;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (board[i] != 'X' && board[i] != 'O') {
            available[count++] = i;
        }
    }

    if (count == 0) return -1;

    uniform_int_distribution<int> distribution(0, count - 1);
    return available[distribution(rng)];
}

// Minimax: υπολογίζει την καλύτερη δυνατή κίνηση
int minimax(Board& board, bool maximizing) {
    if (checkWin(board, 'O')) return 10;
    if (checkWin(board, 'X')) return -10;
    if (checkDraw(board)) return 0;

    if (maximizing) {
        int bestScore = numeric_limits<int>::min();

        for (int i = 0; i < BOARD_SIZE; ++i) {
            if (board[i] != 'X' && board[i] != 'O') {
                char previous = board[i];
                board[i] = 'O';

                int score = minimax(board, false);
                board[i] = previous;

                bestScore = max(bestScore, score);
            }
        }

        return bestScore;
    }

    int bestScore = numeric_limits<int>::max();

    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (board[i] != 'X' && board[i] != 'O') {
            char previous = board[i];
            board[i] = 'X';

            int score = minimax(board, true);
            board[i] = previous;

            bestScore = min(bestScore, score);
        }
    }

    return bestScore;
}

int getBestMove(Board& board) {
    int bestScore = numeric_limits<int>::min();
    int bestMove = -1;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (board[i] != 'X' && board[i] != 'O') {
            char previous = board[i];
            board[i] = 'O';

            int score = minimax(board, false);
            board[i] = previous;

            if (score > bestScore) {
                bestScore = score;
                bestMove = i;
            }
        }
    }

    return bestMove;
}

void displayStatistics(const Statistics& stats) {
    cout << "\n" << CYAN << BOLD;
    cout << "========================================\n";
    cout << "              STATISTICS                \n";
    cout << "========================================\n" << RESET;

    cout << RED << " Player X wins:       " << stats.xWins << RESET << "\n";
    cout << BLUE << " Player O wins:       " << stats.oWins << RESET << "\n";
    cout << GOLD << " Draws:               " << stats.draws << RESET << "\n";

    cout << "\n" << GREEN << BOLD
         << " COMPUTER STATISTICS\n" << RESET;

    cout << " Computer wins:       " << stats.computerWins << "\n";
    cout << " Player wins vs CPU:  " << stats.computerLosses << "\n";
    cout << " Computer draws:      " << stats.computerDraws << "\n";

    cout << CYAN << "========================================\n"
         << RESET;
}

GameMode chooseGameMode() {
    cout << "\n" << BOLD << "SELECT GAME MODE\n" << RESET;
    cout << " 1. Two players\n";
    cout << " 2. Play against computer - Easy\n";
    cout << " 3. Play against computer - Hard\n";
    cout << " 4. View statistics\n";
    cout << " 5. Exit\n\n";

    int choice = getNumber("Your choice: ", 1, 5);

    if (choice == -1) return static_cast<GameMode>(-1);
    if (choice == 1) return GameMode::TwoPlayers;
    if (choice == 2) return GameMode::EasyComputer;
    if (choice == 3) return GameMode::HardComputer;

    return static_cast<GameMode>(choice);
}

void playRound(Statistics& stats, GameMode mode) {
    Board board = createBoard();
    char currentPlayer = 'X';
    bool againstComputer = mode != GameMode::TwoPlayers;
    bool hardMode = mode == GameMode::HardComputer;

    cout << "\n";
    if (mode == GameMode::TwoPlayers) {
        cout << GREEN << "Two-player game started!\n" << RESET;
    } else if (hardMode) {
        cout << RED << BOLD << "Hard mode: the computer plays optimally!\n"
             << RESET;
    } else {
        cout << GREEN << "Easy mode: the computer chooses random moves.\n"
             << RESET;
    }

    while (true) {
        displayBoard(board);

        int move = -1;

        if (againstComputer && currentPlayer == 'O') {
            cout << BLUE << BOLD << " Computer is thinking...\n" << RESET;

            move = hardMode
                ? getBestMove(board)
                : getRandomMove(board);

            if (move == -1) break;

            cout << BLUE << " Computer chooses position "
                 << move + 1 << ".\n" << RESET;
        } else {
            move = getPlayerMove(board, currentPlayer);

            if (move == -1) {
                cout << "\nInput ended. Exiting the game.\n";
                return;
            }

            --move;
        }

        board[move] = currentPlayer;

        if (checkWin(board, currentPlayer)) {
            displayBoard(board);

            cout << GREEN << BOLD
                 << "========================================\n";

            if (againstComputer && currentPlayer == 'O') {
                cout << "          COMPUTER WINS!\n";
                ++stats.computerWins;
                ++stats.oWins;
            } else {
                cout << "       PLAYER " << currentPlayer
                     << " WINS!\n";

                if (currentPlayer == 'X') {
                    ++stats.xWins;
                    if (againstComputer) ++stats.computerLosses;
                } else {
                    ++stats.oWins;
                }
            }

            cout << "========================================\n"
                 << RESET;
            return;
        }

        if (checkDraw(board)) {
            displayBoard(board);

            cout << GOLD << BOLD
                 << "========================================\n"
                 << "                IT'S A DRAW!\n"
                 << "========================================\n"
                 << RESET;

            ++stats.draws;
            if (againstComputer) ++stats.computerDraws;
            return;
        }

        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }
}

int main() {
    enableColors();

    Statistics stats;

    while (true) {
        clearScreen();
        displayHeader();

        cout << " Welcome to Tic-Tac-Toe!\n";
        cout << " Choose a mode and challenge yourself.\n";

        GameMode mode = chooseGameMode();

        if (cin.eof() || cin.fail()) break;

        if (mode == GameMode::TwoPlayers ||
            mode == GameMode::EasyComputer ||
            mode == GameMode::HardComputer) {

            playRound(stats, mode);

            if (cin.eof() || cin.fail()) break;

            displayStatistics(stats);

            if (!askYesNo("\nReturn to main menu? (y/n): ")) {
                break;
            }
        } else if (mode == static_cast<GameMode>(4)) {
            displayStatistics(stats);

            if (!askYesNo("\nReturn to main menu? (y/n): ")) {
                break;
            }
        } else {
            break;
        }
    }

    cout << "\n" << GREEN << BOLD;
    cout << "========================================\n";
    cout << "          THANK YOU FOR PLAYING!        \n";
    cout << "========================================\n";
    cout << RESET;

    displayStatistics(stats);

    return 0;
}
