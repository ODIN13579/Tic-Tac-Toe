#include <iostream>
#include <climits>
#include <unordered_map>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

char board[3][3] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'}
};

const char PLAYER = 'X';
const char COMP = 'O';
int  algorithmChoice = 1;


void show_board() {
    cout << "\n========== Tic Tac Toe ==========\n\n";
    cout << "     |     |     \n";
    cout << "  " << board[0][0] << "  |  " << board[0][1] << "  |  " << board[0][2] << "\n";
    cout << "_____|_____|_____\n";
    cout << "     |     |     \n";
    cout << "  " << board[1][0] << "  |  " << board[1][1] << "  |  " << board[1][2] << "\n";
    cout << "_____|_____|_____\n";
    cout << "     |     |     \n";
    cout << "  " << board[2][0] << "  |  " << board[2][1] << "  |  " << board[2][2] << "\n";
    cout << "     |     |     \n\n";
}


bool checkWin(char mark) {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == mark && board[i][1] == mark && board[i][2] == mark) return true;
        if (board[0][i] == mark && board[1][i] == mark && board[2][i] == mark) return true;
    }
    if (board[0][0] == mark && board[1][1] == mark && board[2][2] == mark) return true;
    if (board[0][2] == mark && board[1][1] == mark && board[2][0] == mark) return true;
    return false;
}

bool isBoardFull() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] != PLAYER && board[i][j] != COMP) return false;
    return true;
}

bool isEmpty(int r, int c) {
    return board[r][c] != PLAYER && board[r][c] != COMP;
}
//  THUẬT TOÁN 1: VÉT CẠN ĐỆ QUY (BRUTE FORCE RECURSIVE)
int bruteForce(bool isMaximizing) {
    if (checkWin(COMP))   return +10;
    if (checkWin(PLAYER)) return -10;
    if (isBoardFull())    return  0;

    if (isMaximizing) {
        int best = INT_MIN;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (isEmpty(i, j)) {
                    char orig = board[i][j];
                    board[i][j] = COMP;
                    best = max(best, bruteForce(false));
                    board[i][j] = orig;   
                }
            }
        }
        return best;
    }
    else {
        int best = INT_MAX;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (isEmpty(i, j)) {
                    char orig = board[i][j];
                    board[i][j] = PLAYER;
                    best = min(best, bruteForce(true));
                    board[i][j] = orig;
                }
            }
        }
        return best;
    }
}

void findBestMove_BruteForce(int& bestRow, int& bestCol) {
    int bestScore = INT_MIN;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (isEmpty(i, j)) {
                char orig = board[i][j];
                board[i][j] = COMP;
                int score = bruteForce(false);
                board[i][j] = orig;
                if (score > bestScore) {
                    bestScore = score;
                    bestRow = i; bestCol = j;
                }
            }
        }
    }
}


//  THUẬT TOÁN 2: QUAY LUI (BACKTRACKING)

bool backtrack(int& bestRow, int& bestCol) {
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (isEmpty(i, j)) {
                board[i][j] = COMP;
                if (checkWin(COMP)) {
                    board[i][j] = (char)('1' + i * 3 + j); 
                    bestRow = i; bestCol = j;
                    return true;
                }
                board[i][j] = (char)('1' + i * 3 + j);
            }
        }
    }

 
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (isEmpty(i, j)) {
                board[i][j] = PLAYER;
                if (checkWin(PLAYER)) {
                    board[i][j] = (char)('1' + i * 3 + j); 
                    bestRow = i; bestCol = j;
                    return true;
                }
                board[i][j] = (char)('1' + i * 3 + j); 
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (isEmpty(i, j)) {
                
                char orig = board[i][j];
                board[i][j] = COMP;

               
                bool goodMove = !checkWin(PLAYER);

                board[i][j] = orig; 

                if (goodMove) {
                    bestRow = i; bestCol = j;
                    return true;
                }
            }
        }
    }

    
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (isEmpty(i, j)) {
                bestRow = i; bestCol = j;
                return true;
            }

    return false;
}


//  THUẬT TOÁN 3: THAM LAM (GREEDY)

void findBestMove_Greedy(int& bestRow, int& bestCol) {
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (isEmpty(i, j)) {
                board[i][j] = COMP;
                if (checkWin(COMP)) {
                    board[i][j] = (char)('1' + i * 3 + j);
                    bestRow = i; bestCol = j;
                    return;
                }
                board[i][j] = (char)('1' + i * 3 + j);
            }
        }
    }

    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (isEmpty(i, j)) {
                board[i][j] = PLAYER;
                if (checkWin(PLAYER)) {
                    board[i][j] = (char)('1' + i * 3 + j);
                    bestRow = i; bestCol = j;
                    return;
                }
                board[i][j] = (char)('1' + i * 3 + j);
            }
        }
    }

    
    if (isEmpty(1, 1)) { bestRow = 1; bestCol = 1; return; }

    
    int corners[4][2] = { {0,0},{0,2},{2,0},{2,2} };
    for (auto& c : corners) {
        if (isEmpty(c[0], c[1])) { bestRow = c[0]; bestCol = c[1]; return; }
    }

    
    int sides[4][2] = { {0,1},{1,0},{1,2},{2,1} };
    for (auto& s : sides) {
        if (isEmpty(s[0], s[1])) { bestRow = s[0]; bestCol = s[1]; return; }
    }
}


//  THUẬT TOÁN 4: QUY HOẠCH ĐỘNG (DYNAMIC PROGRAMMING)

unordered_map<string, int> memo;

string boardToKey(bool isMax) {
    string key = "";
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            key += board[i][j];
    key += (isMax ? 'M' : 'm');
    return key;
}

int dp(bool isMaximizing) {
    if (checkWin(COMP))   return +10;
    if (checkWin(PLAYER)) return -10;
    if (isBoardFull())    return  0;

    string key = boardToKey(isMaximizing);

    
    if (memo.count(key)) return memo[key];

    if (isMaximizing) {
        int best = INT_MIN;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (isEmpty(i, j)) {
                    char orig = board[i][j];
                    board[i][j] = COMP;
                    best = max(best, dp(false));
                    board[i][j] = orig;
                }
            }
        }
        memo[key] = best;
        return best;
    }
    else {
        int best = INT_MAX;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (isEmpty(i, j)) {
                    char orig = board[i][j];
                    board[i][j] = PLAYER;
                    best = min(best, dp(true));
                    board[i][j] = orig;
                }
            }
        }
        memo[key] = best;
        return best;
    }
}

void findBestMove_DP(int& bestRow, int& bestCol) {
    memo.clear(); 
    int bestScore = INT_MIN;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (isEmpty(i, j)) {
                char orig = board[i][j];
                board[i][j] = COMP;
                int score = dp(false);
                board[i][j] = orig;
                if (score > bestScore) {
                    bestScore = score;
                    bestRow = i; bestCol = j;
                }
            }
        }
    }
}


//
void AI_move() {
    int r = -1, c = -1;

    switch (algorithmChoice) {
    case 1:
        cout << "[AI - Brute Force Recursive dang suy nghi...]\n";
        findBestMove_BruteForce(r, c);
        break;
    case 2:
        cout << "[AI - Backtracking dang suy nghi...]\n";
        backtrack(r, c);
        break;
    case 3:
        cout << "[AI - Greedy dang suy nghi...]\n";
        findBestMove_Greedy(r, c);
        break;
    case 4:
        cout << "[AI - Dynamic Programming dang suy nghi...]\n";
        findBestMove_DP(r, c);
        break;
    }

    if (r != -1 && c != -1)
        board[r][c] = COMP;
}

// ============================================================
//  LƯỢT NGƯỜI CHƠI
// ============================================================
void playerMove() {
    int choice;
    while (true) {
        cout << "Player (X) nhap vi tri (1-9): ";
        cin >> choice;
        if (choice < 1 || choice > 9) {
            cout << "  Vui long nhap so tu 1 den 9!\n";
            continue;
        }
        int r = (choice - 1) / 3, c = (choice - 1) % 3;
        if (!isEmpty(r, c)) {
            cout << "  O nay da bi chiem, thu lai!\n";
        }
        else {
            board[r][c] = PLAYER;
            break;
        }
    }
}

// ============================================================
//  MENU CHỌN THUẬT TOÁN
// ============================================================
void showMenu() {
    
    cout << "   CHON THUAT TOAN CHO AI\n";
    
    cout << "  1. Vet can De quy (Brute Force) \n";
    cout << "  2. Quay lui     (Backtracking)   \n";
    cout << "  3. Tham lam     (Greedy)        \n";
    cout << "  4. Quy hoach dong (DP)           \n";
    cout << "-----------------------------------\n";
    cout << "Lua chon cua ban (1-4): ";
    cin >> algorithmChoice;
    if (algorithmChoice < 1 || algorithmChoice > 4) algorithmChoice = 1;

    string names[] = { "", "Brute Force Recursive", "Backtracking", "Greedy", "Dynamic Programming" };
    cout << "\n>> Ban da chon: " << names[algorithmChoice] << "\n";
}


void resetBoard() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = (char)('1' + i * 3 + j);
}


int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    cout << "========== TIC TAC TOE ==========\n";
    cout << "  Ban la X  |  May tinh la O\n";

    char playAgain = 'y';
    while (playAgain == 'y' || playAgain == 'Y') {
        resetBoard();
        memo.clear();

        showMenu();
        cout << "\nNhan Enter de bat dau...";
        cin.ignore(); cin.get();

        bool run = true;
        while (run) {
            show_board();

            // Lượt người
            playerMove();
            show_board();

            if (checkWin(PLAYER)) {
                cout << "\n  Ban thang roi! Xin chuc mung!\n";
                cout << "========== GAME OVER ==========\n";
                break;
            }
            if (isBoardFull()) {
                cout << "\n  Hoa! Ca hai deu choi tot!\n";
                cout << "========== GAME OVER ==========\n";
                break;
            }

            // Lượt máy
            AI_move();
            show_board();

            if (checkWin(COMP)) {
                cout << "\n  May tinh thang! Co gang hon nua nhe!\n";
                cout << "========== GAME OVER ==========\n";
                break;
            }
            if (isBoardFull()) {
                cout << "\n  Hoa! Ca hai deu choi tot!\n";
                cout << "========== GAME OVER ==========\n";
                break;
            }
        }

        cout << "\nChoi lai? (y/n): ";
        cin >> playAgain;
    }

    cout << "\nCam on ban da choi! Hen gap lai!\n";
    return 0;
}
