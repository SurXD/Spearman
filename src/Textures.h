#pragma once
#include <iostream>
#include <cmath>
#include <graphics.h>

using namespace std;

constexpr int BUTTON_MENU_COUNT = 4;
constexpr int ITEMS_COUNT = 9;
constexpr int ENVIROMENT_OBJECTS_COUNT = 20;
constexpr int ENEMY_TYPE_COUNT = 7;

extern IMAGE* aboutScreen; //Изображение "О программе"
extern IMAGE* rulesScreen; //Изображение "Правила"
extern IMAGE* screensaver; //Изображение "Заставка"

extern IMAGE* bmpBASIC[ENVIROMENT_OBJECTS_COUNT]; // массив для различных базовых изображений 
extern IMAGE* bmpITEMS[ITEMS_COUNT]; // массив для изображения предметов и иконок предметов
extern IMAGE* bmpEnemy[ENEMY_TYPE_COUNT]; // массив для изображения врагов 

struct button{ // кнопка
   int  x, y, dx, dy;
   IMAGE * bmp;
};
void draw_menu(); //отрисовка кнопок меню
int menu(); // выбор пункта меню
void load_menu_sprites(); // инициализация меню
void load_items_sprites();
void load_enemy_sprites();

void draw_end_game(int x); // отрисока финального экрана

extern int b; //этаж