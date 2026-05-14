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

void readBoard(int board[9][9]);
void printBoard(int board[9][9]);
bool isValid(int board[9][9], int row, int col, int num);
bool solve(int board[9][9]);
bool isInitialValid(int board[9][9]);

int main()
{
    int board[9][9];
    readBoard(board);

    if (!isInitialValid(board))
        cout << "No solution exists";

    else if(solve(board))
        printBoard(board);

    else
        cout << "No solution exists";
    
    return 0;
}


void readBoard(int board[9][9])
{
    for(int i = 0 ; i < 9 ; i++)
    {
        for(int j = 0 ; j < 9 ; j++)
        {
            cin >> board[i][j];
        }
    }
}

void printBoard(int board[9][9])
{
    for(int i = 0 ; i < 9 ; i++)
    {
        for(int j = 0 ; j < 9 ; j++)
        {
            cout << board[i][j] << ' ';
        }
        cout << endl;
    }
}

bool isValid(int board[9][9], int row, int col, int num)
{
    //checking same row & col
    for(int itterator = 0 ; itterator < 9 ; itterator++)
    {
        if(board[row][itterator] == num || board[itterator][col] == num)
            return false;
    }

    //checking 3*3 sub board

    int start_row = (row / 3) * 3;
    int start_col = (col / 3) * 3;

    for(int i = 0 ; i < 3 ; i++)
    {
        for(int j = 0 ; j < 3 ; j++)
        {
            if(board[start_row + i][start_col + j] == num)
                return false;
        }
    }

    return true;
}

bool solve(int board[9][9])
{
    for(int row = 0 ; row < 9 ; row++)
    {
        for(int col = 0 ; col < 9 ; col++)
        {
            if(board[row][col] == 0)
            {
                for(int num = 1 ; num <= 9 ; num++)
                {
                    if(isValid(board , row , col , num))
                    {
                        board[row][col] = num;

                        if(solve(board))
                            return true;

                        board[row][col] = 0; //backtracking
                    }
                }
                return false;
            }
        }
    }
    return true;
}

bool isInitialValid(int board[9][9])
{
    for(int row = 0 ; row < 9 ; row++)
    {
        for(int col = 0 ; col < 9 ; col++)
        {
            if(board[row][col] != 0)
            {
                int num = board[row][col];
                board[row][col] = 0;

                if(!isValid(board , row , col , num))
                {
                    board[row][col] = num;
                    return false;
                }

                board[row][col] = num;
            }
        }
    }
    return true;
}
