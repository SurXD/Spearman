#pragma once
#include"Textures.h"

struct Enemy{
   int xe, ye; // координаты врага
   int eHP; // здоровье врага
   double startHP; // начальное здоровье врага
   int eSpeed; // скорость врага
   IMAGE * bmpe; // изображение врага
}; // структура врага

typedef struct ES{
   Enemy mas[16][4];
   int nums[16];
} ES; // структура массивов врагов


class Room {
   private:
      Enemy* arr; // массив врагов
      int num; // количество врагов
      int open; //  флаг открытия двери
   public:
      Room(){ // конструктор
         arr = new Enemy[4];
         num = 0;
         open = 0;
         for(int i = 0; i < 4; i++){
            arr[i].xe = 0; arr[i].ye = 0; arr[i].eHP = 0; arr[i].eSpeed = 0;
         }
      }
      ~Room(){ for(int i = 0; i < 4; i++) freeimage(arr[i].bmpe); } //деструктор
      int get_num(){ return num; } // получить количество врагов
      int get_open(){ return open; } //  получить флаг открытия двери
      Enemy* getArr(){ return arr;} // получить массив врагов в комнате
      void initRoom(int x, ES a); // создание комнаты
      void OpenRoom(); // проверка на открытие дверей в комнате
      void Moved(int x, int y); // движение врагов
      void printEnemys(); // отрисовка врагов
}; // класс для комнаты


void init_ES(Enemy normal[]);// инициализация пула комнат