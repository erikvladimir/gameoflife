//
//  GameOfLife.cpp
//
//  Created by Erik V. Ortega on 04/07/2017.
//  Copyright © 2017 Erik V. Ortega. All rights reserved.
//

#include "GameOfLife.hpp"


GameOfLife::GameOfLife(uint height, uint width):
    m_height(height),
    m_width(width),
    m_board(height, vector<int>(width, 0)),
    m_running(true)
{
    // draw initial pattern
    vector<pair<int, int>> pattern = {
        {0,1},
        {1,2},
        {2,0},{2,1},{2,2},
    };

    uint y = 5, x = 5;
    for (auto e : pattern)
    {
        m_board[y + e.first][x + e.second] = 1;
    }
}

GameOfLife::~GameOfLife()
{}

void GameOfLife::toggleCell (uint mouse_y, uint mouse_x)
{
    cout << __FUNCTION__ << endl;
    if ( mouse_y >= m_posy && mouse_y <= (m_posy + m_height) &&
        mouse_x >= m_posx && mouse_x <= (m_posx + m_width))
    {
        lock_guard<mutex> m(m_board_mtx);
        m_board[mouse_y - m_posy][mouse_x - m_posx] ^= true;
    }
}

int GameOfLife::liveNeig(vector<vector<int>>& board, int row, int col)
{
    int count = 0;
    for (int i = std::max(0,row-1); i <= std::min((int)board.size()-1, row+1); i++)
    {
        for (int j = std::max(0,col-1); j <= std::min((int)board[i].size()-1, col+1); j++)
        {
            if (i==row && j==col) continue;
            if (board[i][j]>0) count++;
        }
    }
    return count;
}

// do an in-pace calculation of the next generation
void GameOfLife::nextGen(vector<vector<int>>& board)
{
    if (board.size() == 0 || board[0].size()==0 ) return;

    for (int i=0; i < board.size(); i++)
    {
        for (int j=0; j < board[i].size(); j++)
        {
            int n = liveNeig(board, i, j);

            if (board[i][j]==1) // cell is alive
                board[i][j] = (n>=2 && n<=3)?1:2;  // if =2, it will die next gen.

            if (board[i][j]==0 && n==3)  // cell is dead
                board[i][j] = -1;   // if =-1, it will become alive next gen.
        }
    }


    for (int i=0; i < board.size(); i++)
    {
        for (int j=0; j < board[i].size(); j++)
        {
            if (board[i][j] ==  2) board[i][j]=0; // kill cell
            if (board[i][j] == -1) board[i][j]=1; // make cell alive
        }

    }
}

void GameOfLife::loop()
{
    fflush(stdin);

    MEVENT event;
    do
    {
        // refresh visual output
        draw();

        // take a key
        int c = wgetch(stdscr);

        if (c != ERR)
        {
            if (KEY_MOUSE == c) {
                if (OK == getmouse(&event))
                {
                    toggleCell (event.y, event.x);
                }
            }
            else
            {
                if (tolower(c) == 'x') break;
                if (tolower(c) == 'p') m_running = true;
                if (tolower(c) == 's') m_running = false;
            }
        }


        if (m_running)
        {
            lock_guard<mutex> m(m_board_mtx);
            nextGen(m_board);
        }

    }
    while(true);
}
