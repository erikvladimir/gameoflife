//
//  GameOfLife.hpp
//  tunein - assignment
//
//  Created by Erik V. Ortega on 04/07/2017.
//  Copyright © 2017 Erik V. Ortega. All rights reserved.
//

#ifndef GameOfLife_hpp
#define GameOfLife_hpp

#include <iostream>#include <iostream>
#include <stdio.h>

#include <vector>
#include <cstdio>
#include <cstdlib>
#include <algorithm>

#include <curses.h>
#include <unistd.h>

using namespace std;

class GameOfLife
{
private:
    vector<vector<int>> m_board;
    uint m_posx = 33u;
    uint m_posy = 2u;
    uint m_height, m_width;
    bool m_running;
    mutex m_board_mtx;

protected:


    void initialise();

    void toggleCell (uint mouse_y, uint mouse_x);
    void drawBoard() const;
    void drawMenu() const;

    void drawStatus() const;
    void drawHint() const;

    void drawBanner() const;

    static int liveNeig(vector<vector<int>>& board, int row, int col);

    static void nextGen(vector<vector<int>>& board);

public:
    GameOfLife(uint height, uint width);
    ~GameOfLife();

    void draw() const;

    void loop();
};

#endif /* GameOfLife_hpp */
