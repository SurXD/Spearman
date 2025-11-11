#pragma once
#include"Textures.h"
#include"Room.h"

class Hero {
   private:
      //int fqq;
      int x, y; // координаты игрока
      int Damage; // урон игрока
      int HP; // здоровье игрока
      int LenghtSpear; // длина копья
      int frameSpear; // номер кадра атаки копья
      int directionSpear; // направление атаки
      int frameNoDamage; // номер кадра неуязвимости
      int Speed; // скорость игрока
      IMAGE * Items[4]; // массив иконок предметов игрока
      int nextItem; // номер следующего предмета
   public:
      Hero(){ // конструктор
         //fqq = 0;
         x = 298; 
         y = 300;
         HP = 3;//HP 3
         Speed = 12;
         Damage = 1000;//DAMAGE 10
         LenghtSpear = 50;
         frameSpear = 0;
         directionSpear = 1;
         frameNoDamage = 0;
         nextItem = 0;
      }
      int get_x(){ return x;} //получить координату x игрока
      int get_y(){ return y;} //получить координату y игрока
      int get_HP(){ return HP;} //получить здоровье игрока
      void DamageMe(Enemy arr[4], int num); // проверка на нанесение урона игроку
      void DamageEnemy(Enemy arr[4]); // проверка на нанесение урона врагам
      void Move(); // движение игрока
      void SpearAttack(); // атака игрока
      void DrawHero(); // отрисовка игрока и его копья
      void DrawStats(); // отрисовка характеристик игрока
      int HeroDoor(int f[5], int ID); // перемещение игрока между комнатами и взятие предмета
      void DrawItems(); // отрисовка иконок предметов игрока
}; // класс игрока


void DrawStat(int x, int y, int stat); // отрисовка одной характеристики