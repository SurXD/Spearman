#include"Textures.h"
#include "Room.h"

void Room::OpenRoom() // проверка на открытие дверей в комнат
{
    door_open = true;
    for(int i = 0; i < current_enemy_count; i++)
    {
        if(enemies[i].HP > 0) door_open = false;
    }
}
void Room::Moved(int x, int y){ // движение врагов
   for(int i = 0; i < current_enemy_count; ++i)
   {
      if(enemies[i].HP > 0)
      {
         double xSpeed, ySpeed;
         double xabs, yabs;
         xabs = abs(x - enemies[i].x); yabs = abs(y - enemies[i].y);
         if (xabs == 0 && yabs == 0){
            xSpeed = 0; ySpeed = 0;
         }
         else if(xabs < yabs){
            ySpeed = enemies[i].Speed/sqrt(1+ (xabs/yabs)*(xabs/yabs)); xSpeed = (xabs/yabs) * ySpeed;
         }
         else if(yabs < xabs){
            xSpeed = enemies[i].Speed/sqrt(1+ (yabs/xabs)*(yabs/xabs)); ySpeed = (yabs/xabs) * xSpeed;
         }
         else{
            xSpeed = ySpeed = enemies[i].Speed * sqrt(2.0)/2;
         }
         if (enemies[i].x - x < 0){xSpeed = -xSpeed;}
         if (enemies[i].y - y < 0){ySpeed = -ySpeed;}
         enemies[i].x -= xSpeed;
         enemies[i].y -= ySpeed;
      }
   }
}
void Room::printEnemys(/*Renderer* renderer*/) { // отрисовка врагов
   for(int i = 0; i < current_enemy_count; i++)
   {
      if(enemies[i].HP > 0)
      {
          //renderer->draw(get_enemy_type(enemies[i]), enemies[i].x, enemies[i].y, enemies[i].bmp, TRANSPARENT_PUT);
         putimage(enemies[i].x, enemies[i].y, enemies[i].bmp, TRANSPARENT_PUT);
         setcolor(BLACK);
         rectangle(enemies[i].x-15, enemies[i].y-16, enemies[i].x+46, enemies[i].y-1);
         setfillstyle(1, RED);
         bar(enemies[i].x - 15, enemies[i].y - 16, enemies[i].x - 15 + int((enemies[i].HP/ enemies[i].startHP)*61), enemies[i].y - 1);
      }
   }
}
void Room::initRoom(int x, set_of_rooms a, int current_floor){ // создание комнаты
   if(x == 4 || x == 2)//начальная комната и комната с предметом
   {
      current_enemy_count = 0; door_open = true;
      for(int i = 0; i < 4; i++)
      {
          enemies[i].x = 0; enemies[i].y = 0; enemies[i].HP = 0; enemies[i].Speed = 0;
      }
   }
   else
   { 
      int f = rand() % 3; //id комнаты этажа (кроме босса)
      if(x == 3) f = 3; //если команта с боссом то f = 3
      for(int i = 0; i < a.enemies_count[current_floor * 4 + f - 4]; i++)//проходимся по количеству врагов в комнате
      {
          enemies[i] = a.enemies_location[current_floor * 4 + f - 4][i];
          current_enemy_count = a.enemies_count[current_floor * 4 + f - 4];
      }
   }
}
