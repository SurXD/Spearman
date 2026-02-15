#ifndef TEXTURES_H
#define TEXTURES_H

#include <graphics.h>
#include "resource_manager/resource_manager.h"
#include "input/winapi_input_manager.h"
#include "state.h"
#include <iostream>
#include <cmath>

using namespace std;

// Параметры загрузки изображений ==============================================================
constexpr int COUNT_BUTTON_MENU = 4;
constexpr int COUNT_ITEMS_SPRITES = 9;
constexpr int COUNT_ENVIROMENT_OBJECTS = 20;

// Параметры генерации подземелья ==============================================================
constexpr int HELL = -10;
constexpr int MAX_COUNT_FLOOR = 4;
constexpr int MAX_COUNT_ROOMS = 16;
constexpr int ENEMY_TYPE_COUNT = 7;
constexpr int MAX_COUNT_ENEMY_ROOMS = 3;

// Параметры генерации этажа ===================================================================
constexpr int MAX_COUNT_ITEM = 4;
constexpr int MAX_SIZE_FLOOR = 6;

constexpr int MAX_DIST_TO_END = 2;
constexpr int MAX_DIST_TO_ITEM = 2;
constexpr int MAX_DIST_FROM_ITEM_TO_END = 1;

// Общие параметры отрисовки ===================================================================
constexpr int CENTER_ROOM_X = 328, CENTER_ROOM_Y = 240;
constexpr int WEIGHT_PLAY_AREA = 656, HIGHT_PLAY_AREA = 480;
constexpr int HEIGHT_WALLS = 65;

// Параметры героя =============================================================================
constexpr int MAX_COUNT_FRAME_NO_DAMAGE = 30;
constexpr int WEIGHT_HERO_SPRITE = 31, HEIGHT_HERO_SPRITE = 72,
			  WEIGHT_HERO_HITBOX = 25, HEIGHT_HERO_HITBOX = 66; 

// Параметры инвентаря =========================================================================
constexpr int LEFT_ALIGHMENT = 735, UP_ALIGHMENT = 125, INTERVAL = 50;
constexpr int SIZE_INVENTORY_SPRITE = 70, INVENTORY_AREA = 313;

// Параметры предметов =========================================================================
constexpr int HEART_EFFECT = 2, DUMBBELL_EFFECT = 4, LIGHTNING_EFFECT = 2, STICK_EFFECT = 20;
constexpr int WEIGHT_ITEM_SPRITE = 31, HEIGHT_ITEM_SPRITE = 72;

// Параметры дверных проёмов ===================================================================
constexpr int WEIGHT_DOOR_GAP = 28;
constexpr int THICNESS_DOOR_GAP = 5;
constexpr int WEIGHT_DOOR = 52, HEIGHT_DOOR = 65;
constexpr int SHIFT_EXIT = 2;

// Параметры мини-карты ========================================================================
constexpr int MAP_X = 670, MAP_Y = 15,
			  WEIGHT_MAP_ROOM = 11, HIGHT_MAP_ROOM = 7,
			  WEIGHT_TUNNEL = 10, HIGHT_TUNNEL = 7, THICKNESS_TUNNEL = 3,
			  MAP_ROOM_JAMB = 1;
constexpr int HERO_COLOR = RGB(80, 80, 80);

//Параметры врагов =============================================================================
constexpr int WEIGHT_ENEMY_SPRITE = 31, //imagewidth(sprite) нужны ли явные константы?
			  HEIGHT_ENEMY_SPRITE = 72; //imageheight(sprite)
constexpr int WEIGHT_HEALTH_BAR = 61,
			  HEIGHT_HEALTH_BAR = 15,
			  LIFTING_HEALTH_BAR = 2;
constexpr int WEIGHT_ENEMY_HITBOX = 25, HEIGHT_ENEMY_HITBOX = 66;

// Параметры отрисовки лестницы - перехода между этажами =======================================
constexpr int STAIRS_X = 312, STAIRS_Y = 155, WEIGHT_STAIRS = 31, HEIGHT_STAIRS = 72, STAIRS_JAMB = 5;


extern IMAGE* about_screen; //Изображение "О программе"
extern IMAGE* rules_screen; //Изображение "Правила"
extern IMAGE* screensaver; //Изображение "Заставка"
extern IMAGE* main_screen; //Изображение "Меню игры"
extern resource_manager* res;
extern input_manager* input;

extern state game_state;

extern IMAGE* bmp_basic[COUNT_ENVIROMENT_OBJECTS]; // массив для различных базовых изображений 
extern IMAGE* bmp_items[COUNT_ITEMS_SPRITES]; // массив для изображения предметов и иконок предметов
extern IMAGE* bmp_enemy[ENEMY_TYPE_COUNT]; // массив для изображения врагов 

struct button{ // кнопка
   int  x, y, dx, dy;
   IMAGE * bmp;
};

void draw_menu(); //отрисовка кнопок меню
void menu(); // выбор пункта меню
void load_menu_sprites(); // инициализация меню
void load_items_sprites();
void load_enemy_sprites();

void draw_game_over();
void game_over();
void win();
void draw_win();

#endif // TEXTURES_H