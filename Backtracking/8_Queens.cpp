#include <iostream>

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
    for(int i = 0 ; i<N ; i++)
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
    
    int possibleCondition = 0;

    for(int col = 0 ; col < N ; col++)
    {
        if(isSafe(queens, row, col, board) && board[row][col] != '*')
        {
            queens[row] = col;
            possibleCondition += solve(queens, row + 1, board);
            queens[row] = -1; //backtracking
        }
    }
    return possibleCondition;
}

bool isSafe(int queens[N], int row, int col, char board[N][N])
{
    for(int prevRow = 0 ; prevRow < row ; prevRow++)
    {
        if(col == queens[prevRow]
            || abs(row - prevRow) == abs(col - queens[prevRow]))
        {
            return false;
        }
    }
    return true;
}