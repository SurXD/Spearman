#include "room.h"

array<vector<int>, MAX_COUNT_ROOMS> enemies_in_rooms =
{ {
    //Враги 1 этажа
    {ZOMBIE, ZOMBIE, ZOMBIE},
    {ZOMBIE, ZOMBIE},
    {ZOMBIE, KNIGHT, ZOMBIE},
    {BOSS1},
    //Враги 2 этажа
    {KNIGHT, KNIGHT},
    {ZOMBIE, KNIGHT, KNIGHT},
    {NINJA, KNIGHT},
    {BOSS2},
    //Враги 3 этажа
    {ZOMBIE, ZOMBIE, ZOMBIE, ZOMBIE},
    {NINJA, NINJA},
    {KNIGHT, KNIGHT, NINJA},
    {BOSS3},
    //Враги 4 этажа
    {KNIGHT, KNIGHT, KNIGHT, KNIGHT},
    {KNIGHT, NINJA, NINJA},
    {NINJA, KNIGHT, NINJA},
    {BOSS4}
} };

array<vector<pair<int, int>>, MAX_COUNT_ROOMS> coordinates_of_enemies =
{ {
    {{258, 205}, {298, 205}, {338, 205}},
    {{570, 0}, {57, 340}},
    {{258, 205}, {298, 205}, {338, 205}},
    {{298, 205}},

    {{258, 205}, {338, 205}},
    {{298, 205}, {570, 0}, {57, 340}},
    {{338, 205}, {298, 205}},
    {{298, 205}},

    {{570, 0}, {57, 340}, {570, 340}, {57, 0}},
    {{258, 205}, {338, 205}},
    {{258, 205}, {338, 205}, {298, 205}},
    {{298, 205}},

    {{570, 0}, {57, 340}, {570, 340}, {57, 0}},
    {{258, 205}, {338, 205}, {298, 205}},
    {{258, 205}, {338, 205}, {298, 205}},
    {{298, 205}}
} };

void Room::init_room(const int room_id, const int floor) { // инициализация комнаты
    enemies.clear(); // Костыль генерации
    if (room_id < MAX_COUNT_ENEMY_ROOMS + 1) {
        for (int i = 0; i < enemies_in_rooms[floor * 4 + room_id].size(); ++i) {   //проходимся по количеству врагов в комнате
            enemies.push_back(kinds_of_enemies[enemies_in_rooms[floor * 4 + room_id][i]]);
            enemies[i].set_coordinates(coordinates_of_enemies[floor * 4 + room_id][i]);
        }
    }
}

bool Room::is_cleared() // проверка на открытие дверей в комнате
{
    for (auto& i : enemies)
        if (i.get_hp() > 0) return false;
    return true;
}

void Room::move_enemies(const int hero_x, const int hero_y){ // движение врагов
   for (auto& i : enemies)
       if (i.get_hp() > 0) i.move(hero_x, hero_y);
}

void Room::draw_enemies(/*Renderer* renderer*/) { // отрисовка врагов
    for (auto& i : enemies)
        if (i.get_hp() > 0) i.draw();
}