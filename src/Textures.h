#pragma once
#include "resource_manager/resource_manager.h"
#include <iostream>
#include <cmath>
#include <graphics.h>

using namespace std;

constexpr int BUTTON_MENU_COUNT = 4;
constexpr int ITEMS_COUNT = 9;
constexpr int ENVIROMENT_OBJECTS_COUNT = 20;
constexpr int ENEMY_TYPE_COUNT = 7;

extern IMAGE* about_screen; //Изображение "О программе"
extern IMAGE* rules_screen; //Изображение "Правила"
extern IMAGE* screensaver; //Изображение "Заставка"
extern IMAGE* main_screen; //Изображение "Меню игры"
extern resource_manager* res;

extern IMAGE* bmp_basic[ENVIROMENT_OBJECTS_COUNT]; // массив для различных базовых изображений 
extern IMAGE* bmp_items[ITEMS_COUNT]; // массив для изображения предметов и иконок предметов
extern IMAGE* bmp_enemy[ENEMY_TYPE_COUNT]; // массив для изображения врагов 

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
