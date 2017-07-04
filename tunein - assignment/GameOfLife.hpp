//
//  GameOfLife.hpp
//
//  Created by Erik V. Ortega on 04/07/2017.
//  Copyright © 2017 Erik V. Ortega. All rights reserved.
//

#ifndef GameOfLife_hpp
#define GameOfLife_hpp

#include <iostream>
#include <stdio.h>

#include <vector>
#include <cstdio>
#include <cstdlib>
#include <algorithm>

#include <curses.h>
#include <unistd.h>

using namespace std;

/**
 * This class contains the game logic 
 */
class GameOfLife
{
protected:
    vector<vector<int>> m_board;
    uint m_posx = 33u;
    uint m_posy = 2u;
    uint m_height, m_width;
    bool m_running;
    mutex m_board_mtx;

    void initialise();
    void toggleCell (uint mouse_y, uint mouse_x);
    
    static int liveNeig(vector<vector<int>>& board, int row, int col);
    static void nextGen(vector<vector<int>>& board);

public:
    GameOfLife(uint height, uint width);
    virtual ~GameOfLife();

    virtual void draw() const = 0;

    void loop();
};

#endif /* GameOfLife_hpp */
