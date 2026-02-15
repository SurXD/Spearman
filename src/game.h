#ifndef GAME_H
#define GAME_H

#include "dungeon.h"

class Game {
    Dungeon* dungeon;
public:
    Game();
    void start();
    void logic();
    void draw();
};

#endif // GAME_H