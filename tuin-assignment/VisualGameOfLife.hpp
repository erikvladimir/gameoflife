//
//  VisualGameOfLife.hpp
//
//  Created by Erik V. Ortega on 04/07/2017.
//  Copyright © 2017 Erik V. Ortega. All rights reserved.
//

#ifndef VisualGameOfLife_hpp
#define VisualGameOfLife_hpp

#include "GameOfLife.hpp"

/**
 * This class contains the visual elements of the game
 */
class VisualGameOfLife :
    public GameOfLife
{
    void drawBoard() const;
    void drawMenu() const;
    void drawStatus() const;
    void drawHint() const;
    void drawBanner() const;
    void initialise();

public:
    VisualGameOfLife(uint height, uint width);
    ~VisualGameOfLife();

    virtual void draw() const;
};

#endif /* VisualGameOfLife_hpp */
