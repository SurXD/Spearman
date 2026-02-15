#ifndef ROOM_H
#define ROOM_H

#include "enemy.h"

class Room
{
    //bool cleared;
    vector<Enemy> enemies;
public:
    //Room() : cleared(false){}
    void init_room(const int room_id, const int floor); // создание комнаты
    vector<Enemy>& get_enemies() { return enemies; } // получить массив врагов в комнате

    bool is_cleared();
    void move_enemies(const int hero_x, const int hero_y); // движение врагов
    void draw_enemies(); // отрисовка врагов
};

#endif // ROOM_H