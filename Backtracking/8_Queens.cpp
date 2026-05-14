#include <iostream>

/*
* ========== Naming Convention Guideline ==========
* Class names: PascalCase
* Function names : camelCase
* Variable names : lower_snake_case
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

using namespace std;

const int N = 8;

void readBoard(char board[N][N]);
int solve(int queens[N], int row, char board[N][N]);
bool isSafe(int queens[N], int row, int col, char board[N][N]);

int main()
{
    char board[N][N];
    readBoard(board);

    int queens[N] = {-1};
    int totalSolutions = solve(queens, 0, board);

    cout << totalSolutions << endl;
}

void readBoard(char board[N][N])
{
    for(int i = 0 ; i < N ; i++)
    {
        for(int j = 0 ; j < N ; j++)
        {
            cin >> board[i][j];
        }
    }
}

int solve(int queens[N], int row, char board[N][N])
{
    if(row == N)
        return 1;
    
    int possible_condition = 0;

    for(int col = 0 ; col < N ; col++)
    {
        if(isSafe(queens, row, col, board) && board[row][col] != '*')
        {
            queens[row] = col;
            possible_condition += solve(queens, row + 1, board);
            queens[row] = -1; //backtracking
        }
    }
    return possible_condition;
}

bool isSafe(int queens[N], int row, int col, char board[N][N])
{
    for(int prev_row = 0 ; prev_row < row ; prev_row++)
    {
        if(col == queens[prev_row]
            || abs(row - prev_row) == abs(col - queens[prev_row]))
        {
            return false;
        }
    }
    return true;
}