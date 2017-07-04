//
//  VisualGameOfLife.cpp
//
//  Created by Erik V. Ortega on 04/07/2017.
//  Copyright © 2017 Erik V. Ortega. All rights reserved.
//

#include "VisualGameOfLife.hpp"


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

VisualGameOfLife::VisualGameOfLife(uint height, uint width):
    GameOfLife(height, width)
{
    initialise();
}

VisualGameOfLife::~VisualGameOfLife()
{
    endwin();
}

void VisualGameOfLife::initialise()
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
    init_pair(1, COLOR_WHITE, COLOR_GREEN);
    init_pair(2, COLOR_WHITE, COLOR_RED);
    init_pair(3, COLOR_BLACK, COLOR_WHITE);
    init_pair(4, COLOR_WHITE, COLOR_GREEN);

    cbreak();
    curs_set(0);
    halfdelay(5);
    keypad(stdscr, true);
    mousemask(ALL_MOUSE_EVENTS, NULL);
}


void VisualGameOfLife::drawBoard() const
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

void VisualGameOfLife::drawMenu() const
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

void VisualGameOfLife::drawStatus() const
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

void VisualGameOfLife::drawHint() const
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

void VisualGameOfLife::drawBanner() const
{
    uint y = 2;
    uint x = 2;

    std::vector<std::string> banner = {
        "  __ _  __ _ _ __ ___   ___",
        " / _` |/ _` | '_ ` _ \\  / _ \\ ",
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


void VisualGameOfLife::draw() const
{
    wclear(stdscr);

    drawBoard();
    drawMenu();
    drawStatus();
    drawBanner();
    drawHint();

    refresh();
}
