#pragma once
#include"Textures.h"

extern int current_floor;

constexpr int MAX_ENEMY_COUNT = 4;
constexpr int MAX_ROOMS_COUNT = 16;

struct Enemy
{
   int x, y; // координаты врага
   int HP; // здоровье врага
   double startHP; // начальное здоровье врага
   int Speed; // скорость врага
   IMAGE * bmp; // изображение врага
}; // структура врага

enum{ ZOMBIE = 0, KNIGHT, NINJA };
enum{ BOSS1 = 3, BOSS2, BOSS3, BOSS4 };

struct set_of_rooms
{
   Enemy enemies_location[MAX_ROOMS_COUNT][MAX_ENEMY_COUNT];
   int enemies_count[MAX_ROOMS_COUNT];
};


class Room 
{
      int  current_enemy_count;
      bool door_open;
      Enemy* enemies;
   public:
      Room() : current_enemy_count(0), door_open(false), enemies(new Enemy[MAX_ENEMY_COUNT]){
         for(int i = 0; i < MAX_ENEMY_COUNT; i++)
         {
             enemies[i].x = 0; enemies[i].y = 0; enemies[i].HP = 0; enemies[i].Speed = 0;
         }
      }
      ~Room(){delete[] enemies;}
      int get_num(){ return current_enemy_count; }
      bool get_open(){ return door_open; }
      Enemy* getArr(){ return enemies;} // получить массив врагов в комнате
      void initRoom(int x, set_of_rooms a); // создание комнаты
      void OpenRoom(); // проверка на открытие дверей в комнате
      void Moved(int x, int y); // движение врагов
      void printEnemys(); // отрисовка врагов
};


void init_set_of_rooms(Enemy normal[]);// инициализация пула комнат