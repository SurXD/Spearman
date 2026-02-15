#ifndef HERO_H
#define HERO_H

#include "enemy.h"
#include "audio/audio_manager.h"
#include "input/input_manager.h"
#include "rendering/renderer.h"

enum {HEART = 1, DUMBBELL, LIGHTNING, STICK};

class Hero {
    int x, y; // координаты игрока
    int damage; // урон игрока
    int hp; // здоровье игрока
    int spear_length;
    int frame_spear; // номер кадра атаки копья
    int direction_spear; // направление атаки
    int frame_no_damage; // номер кадра неуязвимости
    int speed; // скорость игрока
    IMAGE * items[4]; // массив иконок предметов игрока
    int next_item; // номер следующего предмета

    void draw_hero(); // отрисовка игрока и его копья
    void draw_stats();
    void draw_items();
public:
    Hero() {
        x = 298;
        y = 300;
        hp = 3; //hit points 3
        speed = 24; //12  24
        damage = 10;// 10
        spear_length = 60;//50
        frame_spear = 0;
        direction_spear = 1;
        frame_no_damage = 0;
        next_item = 0;
    }
    int& get_x(){ return x;}
    int& get_y(){ return y;}
    int get_hp(){ return hp;}
    void receive_damage(vector<Enemy> &enemies);
    void deal_damage(vector<Enemy> &enemies); // Нанесение урона врагам
    void move(); // движение игрока
    void spear_attack(); // атака игрока
    void take_item(int& item_id);
    void draw();
};

#endif // HERO_H