#ifndef FLOOR_H
#define FLOOR_H

#include "room.h"

using namespace std;

enum {ROOM_NOT_EXIST = -1, END_ROOM = 3, START_ROOM, ITEM_ROOM};
enum {DOWN, RIGHT, UP, LEFT};

class Floor
{
    array<array<int, MAX_SIZE_FLOOR>, MAX_SIZE_FLOOR> rooms_id; // структура этажа
    array<array<int, MAX_SIZE_FLOOR>, MAX_SIZE_FLOOR> floor_map; // мини-карта этажа
    array<array<Room, MAX_SIZE_FLOOR>, MAX_SIZE_FLOOR> rooms; 
    array<bool, 4> adjoining_rooms; //массив существования соседних комнат

    int xi, yj; // текущая комната
    int item_id; // ID предмета
    int number;
    void create_way(int current_x, int current_y, const int end_x, const int end_y); // прокладывает дорогу от одной комнаты до другой
    int get_direction(int& hero_x, int& hero_y);
    bool is_stairs(const int& hero_x, const int& hero_y);
public:
    void init_floor(const int n); // создание этажа

    int get_room_id(){ return rooms_id[xi][yj];} // получить тип комнаты
    int& get_item_id() {return item_id;} // получить ID предмета
    vector<Enemy>& get_enemies() { return rooms[xi][yj].get_enemies(); }
    
    void draw_map();
    void draw_doors();
    void draw_stairs();
    void draw_items();
    void draw_enemies() { rooms[xi][yj].draw_enemies(); }

    void check_of_rooms();
    void move_enemies(const int hero_x, const int hero_y) { rooms[xi][yj].move_enemies(hero_x, hero_y); }
    void change_room(int& hero_x, int& hero_y);
    
    bool room_is_open(){ return rooms[xi][yj].is_cleared(); }
    bool is_completed(const int& hero_x, const int& hero_y);
};

#endif // FLOOR_H