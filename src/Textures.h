#pragma once
#include <iostream>
#include <cmath>
#include <graphics.h>

using namespace std;

struct button{ // кнопка
   int  x, y, dx, dy;
   IMAGE * bmp;
};
void draw_menu(); //отрисовка кнопок меню
int menu(); // выбор пункта меню
void init_menu(); // инициализация меню
void init_items_images();

void draw_end_game(int x); // отрисока финального экрана

extern int b; //этаж
extern IMAGE * bmpEnemy[7]; // массив для изображения врагов
extern IMAGE * bmpITEMS[9]; // массив для изображения предметов и иконок предметов
extern IMAGE * bmpBASIC[20]; // массив для различных базовых изображений 