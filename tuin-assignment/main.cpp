//
//  main.cpp
//
//  Created by Erik V. Ortega on 04/07/2017.
//  Copyright © 2017 Erik V. Ortega. All rights reserved.
//

#include "VisualGameOfLife.hpp"

int main(int argc, const char * argv[])
{
    GameOfLife * game = new VisualGameOfLife(25,25);

    game->loop();

    delete game;

    return 0;
}
