//
//  main.cpp
//  tunein - assignment
//
//  Created by Erik V. Ortega on 04/07/2017.
//  Copyright © 2017 Erik V. Ortega. All rights reserved.
//

#include <iostream>

#include <vector>
#include <cstdio>
#include <cstdlib>
#include <algorithm>

#include <curses.h>
#include <unistd.h>

using namespace std;


void rectangle(int y1, int x1, int y2, int x2)
{
    mvhline(y1, x1, 0, x2-x1);
    mvhline(y2, x1, 0, x2-x1);
    mvvline(y1, x1, 0, y2-y1);
    mvvline(y1, x2, 0, y2-y1);
    mvaddch(y1, x1, ACS_ULCORNER);
    mvaddch(y2, x1, ACS_LLCORNER);
    mvaddch(y1, x2, ACS_URCORNER);
    mvaddch(y2, x2, ACS_LRCORNER);
}

class GameOfLife
{
private:
    vector<vector<int>> m_board;
    uint m_posx = 33u;
    uint m_posy = 2u;
    uint m_height, m_width;
    bool m_running;
    mutex m_board_mtx;

public:
    GameOfLife(uint height, uint width):
        m_height(height),
        m_width(width),
        m_board(height, vector<int>(width, 0)),
        m_running(true)
    {
        initialise();
    }

    ~GameOfLife()
    {
        endwin();
    }

    void initialise()
    {
        setlocale (LC_ALL, "");
        initscr();

        if(has_colors() == false)
        {	endwin();
            printf("Your terminal does not support color.\n");
            exit(1);
        }

        start_color();
        noecho();

        init_pair(0, COLOR_WHITE, COLOR_BLACK);
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        init_pair(2, COLOR_WHITE, COLOR_RED);
        init_pair(3, COLOR_BLACK, COLOR_WHITE);
        init_pair(4, COLOR_WHITE, COLOR_GREEN);

        cbreak();
        curs_set(0);
        halfdelay(5);
        keypad(stdscr, true);
        mousemask(ALL_MOUSE_EVENTS, NULL);

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

    void toggleCell (uint mouse_y, uint mouse_x)
    {
        cout << __FUNCTION__ << endl;
        if ( mouse_y >= m_posy && mouse_y <= (m_posy + m_height) &&
             mouse_x >= m_posx && mouse_x <= (m_posx + m_width))
        {
            lock_guard<mutex> m(m_board_mtx);
            m_board[mouse_y - m_posy][mouse_x - m_posx] ^= true;
        }
    }

    void drawBoard() const
    {
        const static string full_cell = "\u25A0";
        const static string empty_cell = "\u25A1";

        move(0, 20);

        attron(COLOR_PAIR(0));

        rectangle(0,  0, 28, 80);

        rectangle(m_posy-1,  m_posx-1, m_posy+25, m_posx+25);

        uint posy = m_posy, posx = m_posx;
        uint l = 0, c = 0;
        for (auto & line : m_board)
        {
            c = 0;
            for (auto & cell : line)
            {
                move(posy + l, posx + c);
                if (cell > 0)
                {
                    attron(COLOR_PAIR(1));
                    printw( full_cell.c_str() );
                    attroff(COLOR_PAIR(1));
                }
                else
                {
                    printw( empty_cell.c_str() );
                }
                c++;
            }
            l++;
            cout << endl;
        }
        attroff(COLOR_PAIR(0));

    }

    void drawMenu() const
    {
        const uint line = m_posy + 2;
        const uint column = m_posx + 30;

        rectangle(line-1, column-2, line+8, column+15);

        attron(COLOR_PAIR(0));
        move(line+0, column);
        printw(" MENU ");
        attroff(COLOR_PAIR(0));

        attron(COLOR_PAIR(4));
        move(line+2, column);
        printw(" [P] : PLAY  ");
        attroff(COLOR_PAIR(4));

        attron(COLOR_PAIR(2));
        move(line+4, column);
        printw(" [S] : STOP  ");
        attroff(COLOR_PAIR(2));

        attron(COLOR_PAIR(3));
        move(line+6, column);
        printw(" [X] : EXIT  ");
        attroff(COLOR_PAIR(3));
    }

    void drawStatus() const
    {
        const uint line = m_posy + 12;
        const uint column = m_posx + 30;

        rectangle(line-1, column-2, line+1, column+15);

        attron(COLOR_PAIR(0));
        move(line+0, column);
        char aux[20];
        sprintf(aux, "Status: %s", m_running?"running":"pause" );
        printw(aux);
        attroff(COLOR_PAIR(0));
    }

    void drawHint() const
    {
        const uint line = m_posy + 18;
        const uint column = m_posx + 30;

        rectangle(line-1, column-2, line+4, column+15);

        attron(COLOR_PAIR(0));
        move(line+0, column); printw("Hint:" );
        move(line+1, column); printw(" Stop and click" );
        move(line+2, column); printw(" on a cell to " );
        move(line+3, column); printw(" toggle it " );
        attroff(COLOR_PAIR(0));
    }

    void drawBanner() const
    {
        uint y = 2;
        uint x = 2;

        std::vector<std::string> banner = {
            "  __ _  __ _ _ __ ___   ___",
            " / _` |/ _` | '_ ` _ \  / _ \ ",
            "| (_| | (_| | | | | | |  __/ ",
            " \\__, |\\__,_|_| |_| |_|\\___| ",
            "  __/ | ",
            " |___/ ",
            "                 __ ",
            "                / _| ",
            "           ___ | |_ ",
            "          / _ \\|  _| ",
            "         | (_) | | ",
            "          \\___/|_| ",
            " ",
            " ",
            "         _ _  __ ",
            "         | (_)/ _| ",
            "         | |_| |_ ___ ",
            "         | | |  _/ _ \\ ",
            "         | | | ||  __/ ",
            "         |_|_|_| \\___| ",
        };

        attron(COLOR_PAIR(0));
        for (auto s : banner)
        {
            move(y++, x);
            printw(s.c_str());
        }
        attroff(COLOR_PAIR(0));
    }

    static int liveNeig(vector<vector<int>>& board, int row, int col)
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
    static void nextGen(vector<vector<int>>& board)
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

public:
    void draw() const
    {
        wclear(stdscr);

        drawBoard();
        drawMenu();
        drawStatus();
        drawBanner();
        drawHint();

        refresh();
    }

    void loop()
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
};

int main(int argc, const char * argv[])
{
    GameOfLife game(25,25);

    game.loop();

    return 0;
}
