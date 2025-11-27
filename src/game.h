#ifndef GAME_H
#define GAME_H

#include "Hero.h"
#include "Stage.h"

using namespace std;

class game
{
    int current_floor = 1; // этаж
    set_of_rooms a; // пул комнат
    Enemy norm[7] = {
        {298, 205, 100, 100, 4, bmp_enemy[ZOMBIE]},//ZOMBIE
        {298, 205, 200, 200, 6, bmp_enemy[KNIGHT]}, //KNIGHT
        {298, 205, 50, 50, 8, bmp_enemy[NINJA]}, //NINJA
        {298, 205, 1000, 1000, 5, bmp_enemy[BOSS1]}, //BOSS1
        {298, 205, 700, 700, 7, bmp_enemy[BOSS2]}, //BOSS2
        {298, 205, 200, 200, 9, bmp_enemy[BOSS3]}, //BOSS3
        {298, 205, 1000, 1000, 9, bmp_enemy[BOSS4]}}; //BOSS4
    Room* Room;
    Hero* Hero;
    Stage* Stage;

    void init_set_of_rooms(Enemy normal[]);
public:
    game();
    void start();
    void logic();
    void draw();
};

#endif // GAME_H
