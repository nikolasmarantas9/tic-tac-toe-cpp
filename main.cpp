#include <iostream>
#include <vector>
#include <limits>

using namespace std;

// Συνάρτηση για την εμφάνιση του ταμπλό
void displayBoard(const vector<char>& board) {
    cout << "\n";
    cout << "  " << board[0] << " | " << board[1] << " | " << board[2] << " \n";
    cout << " ---+---+---\n";
    cout << "  " << board[3] << " | " << board[4] << " | " << board[5] << " \n";
    cout << " ---+---+---\n";
    cout << "  " << board[6] << " | " << board[7] << " | " << board[8] << " \n";
    cout << "\n";
}

// Συνάρτηση ελέγχου νικητή
bool checkWin(const vector<char>& board, char player) {
    int winPatterns[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // Οριζόντιοι
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // Κάθετοι
        {0, 4, 8}, {2, 4, 6}             // Διαγώνιοι
    };

    for (const auto& pattern : winPatterns) {
        if (board[pattern[0]] == player && board[pattern[1]] == player && board[pattern[2]] == player) {
            return true;
        }
    }
    return false;
}

// Συνάρτηση ελέγχου ισοπαλίας
bool checkDraw(const vector<char>& board) {
    for (char cell : board) {
        if (cell != 'X' && cell != 'O') {
            return false; 
        }
    }
    return true;
}

int main() {
    char playAgain;
    do {
        vector<char> board = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
        char currentPlayer = 'X';
        bool gameEnded = false;

        cout << "=================================\n";
        cout << "       TIC-TAC-TOE (TRILIZA)     \n";
        cout << "=================================\n";

        while (!gameEnded) {
            displayBoard(board);
            cout << "Player " << currentPlayer << ", choose a position (1-9): ";
            
            int choice;
            cin >> choice;

            // Έλεγχος εγκυρότητας επιλογής
            if (cin.fail() || choice < 1 || choice > 9 || board[choice - 1] == 'X' || board[choice - 1] == 'O') {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid move! Please try again.\n";
                continue;
            }

            // Τοποθέτηση συμβόλου
            board[choice - 1] = currentPlayer;

            // Έλεγχος νίκης ή ισοπαλίας
            if (checkWin(board, currentPlayer)) {
                displayBoard(board);
                cout << "Congratulations! Player " << currentPlayer << " wins!\n";
                gameEnded = true;
            } else if (checkDraw(board)) {
                displayBoard(board);
                cout << "It's a draw!\n";
                gameEnded = true;
            } else {
                currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
            }
        }

        cout << "\nDo you want to play again? (y/n): ";
        cin >> playAgain;
        cout << "\n---------------------------------\n";

    } while (playAgain == 'y' || playAgain == 'Y');

    cout << "Thanks for playing!\n";
    return 0;
}