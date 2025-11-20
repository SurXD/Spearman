#pragma once
#include <array>

constexpr int MAX_ITEM_COUNT = 4;
constexpr int MAX_STAGE_SIZE = 6;

constexpr int MAX_DIST_TO_END = 2;
constexpr int MAX_DIST_TO_ITEM = 2;
constexpr int MAX_DIST_FROM_ITEM_TO_END = 1;

enum {ROOM_NOT_EXIST, ENEMY_ROOM, ITEM_ROOM, END_ROOM, EMPTY_ROOM};
enum {UP, RIGHT, DOWN, LEFT};

class Stage
{
      array<array<int, MAX_STAGE_SIZE>, MAX_STAGE_SIZE> stage; // структура этажа
      array<array<int, MAX_STAGE_SIZE>, MAX_STAGE_SIZE> stagemap; // мини-карта этажа
      array<bool, 4> adjoining_rooms; //массив существования соседних комнат
      int xi, yj; // текущая комната
      int ItemID; // ID предмета
      void create_way(int x_current, int y_current, const int x_end, const int y_end); // прокладывает дорогу с обычными врагами от одной комнаты до другой
   public:
      int getRoom(){ return stage[xi][yj];} // получить тип комнаты
      int getItemID() {return ItemID;} // получить ID предмета
      array<bool, 4> get_adj_rooms() { return adjoining_rooms; } // получить массив смежных комнат
      void printMap(); // отрисовка мини-карты
      void initStage(); // создание этажа
      void printDoorsAndItems(bool doors); // отрисовка дверей и предметов 
      int StageMove(int direction); // перемещение игрока по этажу и взятие предмета
      void check_of_rooms(); // отмечает смежные комнаты
}; // класс для этажа