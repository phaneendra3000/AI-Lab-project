#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const int N = 8; // Board size

bool isSafe(vector<int>& board, int row, int col) {
    for (int i = 0; i < row; ++i)
        if (board[i] == col || abs(i - row) == abs(board[i] - col))
            return false;
    return true;
}

bool reviseConstraints(vector<vector<bool>>& constraints, int xi, int xj) {
    bool revised = false;
    for (int i = 0; i < N; ++i) {
        if (constraints[xi][i]) {
            bool hasValid = false;
            for (int j = 0; j < N; ++j) {
                if (constraints[xj][j] && j != i) {
                    hasValid = true;
                    break;
                }
            }
            if (!hasValid) {
                constraints[xi][i] = false;
                revised = true;
            }
        }
    }
    return revised;
}

bool AC3(vector<vector<bool>>& constraints) {
    queue<pair<int, int>> q;

    for (int i = 0; i < N - 1; ++i) {
        for (int j = i + 1; j < N; ++j) {
            q.push(make_pair(i, j));
        }
    }

    while (!q.empty()) {
        pair<int, int> pair_ij = q.front();
        int xi = pair_ij.first;
        int xj = pair_ij.second;
        q.pop();

        if (reviseConstraints(constraints, xi, xj)) {
            for (int k = 0; k < N; ++k) {
                if (k != xi && constraints[k][xi]) {
                    q.push(make_pair(k, xi));
                }
            }
        }
    }

    for (int i = 0; i < N; ++i) {
        bool found = false;
        for (int j = 0; j < N; ++j) {
            if (constraints[i][j]) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
    }

    return true;
}

bool solveNQueens(vector<int>& board, int row, vector<vector<bool>>& constraints) {
    if (row == N) {
        // Solution found
        for (int i = 0; i < N; ++i) {
            cout << board[i] << " ";
        }
        cout << endl;
        return true;
    }

    for (int i = 0; i < N; ++i) {
        if (constraints[row][i]) {
            if (isSafe(board, row, i)) {
                board[row] = i;
                vector<vector<bool>> updated_constraints = constraints;

                for (int j = row + 1; j < N; ++j) {
                    if (updated_constraints[j][i])
                        updated_constraints[j][i] = false;

                    int d = j - row;
                    if (i + d < N)
                        updated_constraints[j][i + d] = false;
                    if (i - d >= 0)
                        updated_constraints[j][i - d] = false;
                }

                if (AC3(updated_constraints) && solveNQueens(board, row + 1, updated_constraints))
                    return true;

                board[row] = -1; // Backtrack
            }
        }
    }

    return false;
}

int main() {
    vector<int> board(N, -1);
    vector<vector<bool>> constraints(N, vector<bool>(N, true));

    if (!solveNQueens(board, 0, constraints)) {
        cout << "No solution exists." << endl;
    }

    return 0;
}
