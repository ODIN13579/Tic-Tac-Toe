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
int algorithmChoice = 1;

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

bool isBoardFull() {// Kiểm tra xem bàn cờ đã đầy chưa
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] != PLAYER && board[i][j] != COMP) return false;
    return true;
}

bool isEmpty(int r, int c) {
    return board[r][c] != PLAYER && board[r][c] != COMP;// Kiểm tra ô có trống không
}

// Khôi phục ô về ký tự số ban đầu
inline char cellChar(int r, int c) { return (char)('1' + r * 3 + c); }// Giúp hoàn tác nước đi khi quay lùi


// ============================================================
//  THUẬT TOÁN 1: QUAY LUI. (BACKTRACKING)
// ============================================================
bool backtrackPure(bool isMaximizing) {// true máy , false người
    if (checkWin(COMP))   return true;// Máy đã thắng
    if (checkWin(PLAYER)) return false;// Người đã thắng
    if (isBoardFull())    return false;// Hòa, không ai thắng

    if (isMaximizing) {// lượt của máy 
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (isEmpty(i, j)) {
                    board[i][j] = COMP;// tại vị trí trống, máy thử đặt O

                    if (backtrackPure(false) == true ) {// 
                        board[i][j] = cellChar(i, j);
                        return true;
                    }
                    board[i][j] = cellChar(i, j);
                }
         return false;
    } 
    else {// Đối thủ đặt X, AI sẽ thử tất cả nước đi của đối thủ
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (isEmpty(i, j)) {
                    board[i][j] = PLAYER;

                    if (backtrackPure(true) == false) {
                        board[i][j] = cellChar(i, j);
                        return false;
                    }

                    board[i][j] = cellChar(i, j);
                }
        return true;
    }
}
void findBestMove_Backtrack(int& bestRow, int& bestCol) {// Tìm nước đi tốt nhất cho AI bằng thuật toán quay lui thuần túy
    //  Máy thắng ngay nếu có
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (isEmpty(i, j)) {
                board[i][j] = COMP;
                bool win = checkWin(COMP);
                board[i][j] = cellChar(i, j);
                if (win) {
                    bestRow = i;
                    bestCol = j;
                    return;
                }
            }
    // Chặn người nếu sắp thắng
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (isEmpty(i, j)) {
                board[i][j] = PLAYER;
                bool win = checkWin(PLAYER);
                board[i][j] = cellChar(i, j);
                if (win) {
                    bestRow = i;
                    bestCol = j;
                    return;
                }
            }

    // Dùng quay lui tìm đường thắng 
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (isEmpty(i, j)) {
                board[i][j] = COMP;

                if (backtrackPure(false)) {
                    bestRow = i;
                    bestCol = j;
                    board[i][j] = cellChar(i, j);
                    return;
                }
                board[i][j] = cellChar(i, j);
            }
    // fallback (chọn ô đầu tiên)
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (isEmpty(i, j)) {
                bestRow = i;
                bestCol = j;
                return;
            }
}

// ============================================================
//  THUẬT TOÁN 2: THAM LAM (GREEDY)
//  - Chỉ nhìn 1 bước, chọn nước tốt nhất ngay lập tức
//  - Ưu tiên: Thắng ngay > Chặn đối thủ > Trung tâm > Góc > Cạnh
//  - Không nhìn xa, có thể thua nếu đối thủ đặt bẫy 2 bước
// ============================================================
void findBestMove_Greedy(int& bestRow, int& bestCol) {
    // Bước 1: Thắng ngay nếu có thể
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (isEmpty(i, j)) {
                board[i][j] = COMP;
                bool win = checkWin(COMP);
                board[i][j] = cellChar(i, j);
                if (win) { bestRow = i; bestCol = j; return; }
            }

    // Bước 2: Chặn nếu đối thủ sắp thắng
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (isEmpty(i, j)) {
                board[i][j] = PLAYER;
                bool win = checkWin(PLAYER);
                board[i][j] = cellChar(i, j);
                if (win) { bestRow = i; bestCol = j; return; }
            }

    // Bước 3: Chọn trung tâm
    if (isEmpty(1, 1)) { bestRow = 1; bestCol = 1; return; }

    // Bước 4: Chọn góc
    int corners[4][2] = { {0,0},{0,2},{2,0},{2,2} };
    for (auto& c : corners)
        if (isEmpty(c[0], c[1])) { bestRow = c[0]; bestCol = c[1]; return; }

    // Bước 5: Chọn cạnh
    int sides[4][2] = { {0,1},{1,0},{1,2},{2,1} };
    for (auto& s : sides)
        if (isEmpty(s[0], s[1])) { bestRow = s[0]; bestCol = s[1]; return; }
}

// ============================================================
//  THUẬT TOÁN 3: QUY HOẠCH ĐỘNG (DYNAMIC PROGRAMMING)
//  - Giống Đệ quy nhưng lưu kết quả đã tính vào bảng memo
//  - Nếu cùng trạng thái bàn cờ thì dùng lại, không tính lại
//  - Tiết kiệm thời gian khi gặp trạng thái lặp lại
// ============================================================
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
    if (memo.count(key)) return memo[key];  // Tra bảng nhớ

    if (isMaximizing) {
        int best = INT_MIN;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (isEmpty(i, j)) {
                    board[i][j] = COMP;
                    best = max(best, dp(false));
                    board[i][j] = cellChar(i, j);
                }
        return memo[key] = best;  // Lưu vào bảng nhớ
    }
    else {
        int best = INT_MAX;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (isEmpty(i, j)) {
                    board[i][j] = PLAYER;
                    best = min(best, dp(true));
                    board[i][j] = cellChar(i, j);
                }
        return memo[key] = best;  // Lưu vào bảng nhớ
    }
}

void findBestMove_DP(int& bestRow, int& bestCol) {
    memo.clear();
    int bestScore = INT_MIN;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (isEmpty(i, j)) {
                board[i][j] = COMP;
                int score = dp(false);
                board[i][j] = cellChar(i, j);
                if (score > bestScore) {
                    bestScore = score;
                    bestRow = i; bestCol = j;
                }
            }
}

// ============================================================
//  THUẬT TOÁN 4: MINIMAX + ALPHA-BETA PRUNING (TỐI ƯU NHẤT)
//  - Minimax: duyệt cây đầy đủ, MAX muốn điểm cao, MIN muốn thấp
//  - Alpha-Beta: cắt tỉa các nhánh chắc chắn không ảnh hưởng kết quả
//    + alpha: điểm tốt nhất MAX đảm bảo được (khởi đầu = -∞)
//    + beta : điểm tốt nhất MIN đảm bảo được (khởi đầu = +∞)
//    + Nếu beta <= alpha: nhánh này không cần xét thêm → CẮT
//  - Kết quả y hệt Đệ quy nhưng nhanh hơn rất nhiều
// ============================================================
int minimax(bool isMaximizing, int alpha, int beta, int depth) {
    if (checkWin(COMP))   return +10 - depth;
    if (checkWin(PLAYER)) return -10 + depth;
    if (isBoardFull())    return  0;

    if (isMaximizing) {
        int best = INT_MIN;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (isEmpty(i, j)) {
                    board[i][j] = COMP;
                    int val = minimax(false, alpha, beta, depth + 1);
                    board[i][j] = cellChar(i, j);
                    best = max(best, val);
                    alpha = max(alpha, best);
                    if (beta <= alpha) goto done_max;  
                }
    done_max:
        return best;
    }
    else {
        int best = INT_MAX;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (isEmpty(i, j)) {
                    board[i][j] = PLAYER;
                    int val = minimax(true, alpha, beta, depth + 1);
                    board[i][j] = cellChar(i, j);
                    best = min(best, val);
                    beta = min(beta, best);
                    if (beta <= alpha) goto done_min; 
                }
    done_min:
        return best;
    }
}

void findBestMove_Minimax(int& bestRow, int& bestCol) {
    int bestScore = INT_MIN;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (isEmpty(i, j)) {
                board[i][j] = COMP;
                int score = minimax(false, INT_MIN, INT_MAX, 0);
                board[i][j] = cellChar(i, j);
                if (score > bestScore) {
                    bestScore = score;
                    bestRow = i; bestCol = j;
                }
            }
}


void AI_move() {
    int r = -1, c = -1;
    switch (algorithmChoice) {
    case 1:
        cout << "[AI - Quay lui (Backtracking) dang suy nghi...]\n";
        findBestMove_Backtrack(r, c);
        break;
    case 2:
        cout << "[AI - Tham lam (Greedy) dang suy nghi...]\n";
        findBestMove_Greedy(r, c);
        break;
    case 3:
        cout << "[AI - Quy hoach dong (DP) dang suy nghi...]\n";
        findBestMove_DP(r, c);
        break;
    case 4:
        cout << "[AI - Minimax + Alpha-Beta Pruning dang suy nghi...]\n";
        findBestMove_Minimax(r, c);
        break;
    }
    if (r != -1 && c != -1)
        board[r][c] = COMP;
}

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
        if (!isEmpty(r, c))
            cout << "  O nay da bi chiem, thu lai!\n";
        else {
            board[r][c] = PLAYER;
            break;
        }
    }
}

void showMenu() {
    cout << "     CHON THUAT TOAN CHO AI\n";
    cout << "  1. Quay lui          (Backtracking)\n";
    cout << "  2. Tham lam          (Greedy)\n";
    cout << "  3. Quy hoach dong    (DP)\n";
    cout << "  4. Minimax          \n";
    cout << "-----------------------------------\n";
    cout << "Lua chon cua ban (1-4): ";
    cin >> algorithmChoice;
    if (algorithmChoice < 1 || algorithmChoice > 4) algorithmChoice = 4;

    string names[] = { "", "Quay lui (Backtracking)",
                       "Tham lam (Greedy)", "Quy hoach dong (DP)",
                       "Minimax + Alpha-Beta Pruning" };
    cout << "\n>> Ban da chon: " << names[algorithmChoice] << "\n";

    if (algorithmChoice == 2)
        cout << "   [Ghi chu: Greedy co the bi danh bai!]\n";
    else if (algorithmChoice == 4)
        cout << "   [Ghi chu: AI nay bat bai - tot nhat la hoa!]\n";
}

void resetBoard() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = cellChar(i, j);
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

        while (true) {
            show_board();

            // Lượt người chơi
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

            // Lượt AI
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
