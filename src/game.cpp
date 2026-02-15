#include "game.h"

Game::Game() : dungeon(new ::Dungeon){}

void Game::start() {
    delete dungeon;

    srand(time(0));
    dungeon = new ::Dungeon;
}

void Game::logic() {
    int is_end = dungeon->change_state();
    if(is_end < 0 || is_end >= MAX_COUNT_FLOOR) {
        if(is_end == HELL) game_state = state::GAME_OVER;
        else game_state = state::WIN;
    }
}

void Game::draw() {
    putimage(0, 0, bmp_basic[17], TRANSPARENT_PUT);
    dungeon->draw();
}
