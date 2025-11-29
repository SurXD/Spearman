#include "Textures.h"
#include"Hero.h"
#include"Room.h"
#include "audio/audio_manager.h"
#include "input/input_manager.h"
#include <array>

extern audio_manager* audio;
extern input_manager* input;

using namespace std;

int Hero::HeroDoor(array<bool, 4> f, int ID, int type_room){ // перемещение игрока между комнатами и взятие предмета
   if(type_room == 3){
      if(298 > x - 31 && 298 < x + 31 && 155 < y + 72 && 155 > y - 72) return 10;
   }
   else{ 
      if(f[0] && x > 296 && x < 356 && y > 337){//нижняя дверь
         y = 10; return 0;
      }
      else if(f[1] && y > 163 && y < 200 && x > 567){//дверь справа
         x = 67; return 1;
      }
      else if(f[2] && x > 296 && x < 356 && y < 5){//верхняя дверь
         y = 330; return 2; 
      }
      else if(f[3] && y > 163 && y < 200 && x < 60){//дверь слева
         x = 560; return 3;
      }
   }
   if(ID && type_room == 2){
      if(315 > x - 31 && 315 < x + 31 && 213 < y + 72 && 213 > y - 72){
         if(ID == 1) HP += 2;
         if(ID == 2) Damage += 4;
         if(ID == 3) Speed += 2;
         if(ID == 4) LenghtSpear += 20;
         Items[nextItem] = bmp_items[2*ID - 2];
         nextItem +=1;
         return 11;
      }
   }
   return -1;
}
void Hero::DamageMe(Enemy arr[4], int num){ // проверка на нанесение урона игроку
   int f = 0;
   for(int i = 0; i < num; i++){
      if(arr[i].HP > 0){
          if (arr[i].x > x - 25 && arr[i].x < x + 25 && arr[i].y < y + 66 && arr[i].y > y - 66) 
          {
              f = 1;
          }
      }
   }
   
   if(frameNoDamage > 0) frameNoDamage++; 
   if(frameNoDamage == 0 && f == 1){
      HP -= 1; 
      frameNoDamage = 1;
      audio->play_sound({ "./Resources/Sound/Hit.wav" });
   }
   if(frameNoDamage == 30) frameNoDamage = 0;
}
void Hero::DamageEnemy(Enemy arr[4]){ // проверка на нанесение урона врагам
   if(frameSpear == 19){
      int xa, ya;
      if(directionSpear == 1){ 
         xa = x; 
         ya = y + 37 + LenghtSpear + frameSpear;
      }
      else if(directionSpear == 2){ 
         xa = x + LenghtSpear + frameSpear; 
         ya = y + 47;
      }
      else if(directionSpear == 3){ 
         xa = x + 22;
         ya = y + 27 - LenghtSpear - frameSpear;
      }
      else if(directionSpear == 4){ 
         xa = x + 21 - LenghtSpear - frameSpear;
         ya = y + 47;
      }
      for(int i = 0; i < 5; i++){
            if(arr[i].HP > 0 && xa > arr[i].x - 9 && xa < arr[i].x + 31 && ya < arr[i].y + 72 && ya > arr[i].y - 9){
            arr[i].HP -= Damage;
         }
      }
   }
}
void Hero::Move()// движение игрока
{
   
    if(input->is_pressed(key::W))
    {
        y = max(0, y - Speed/2);
    }
    if(input->is_pressed(key::A))
    {
        x = max(57, x - Speed/2);
    }
    if(input->is_pressed(key::S))
    {
        y = min(340, y + Speed/2);
    }
    if(input->is_pressed(key::D))
    {
        x = min(570, x + Speed/2);
    }
}
void Hero::SpearAttack(){ // атака игрока
   if(frameSpear == 0)
   {

      std::array<key, 4> keykodes{ key::DOWN, key::RIGHT, key::UP, key::LEFT };

      for(int i = 0; i < keykodes.size(); i++)
      {
          if(input->is_pressed(keykodes[i]))
          {
              frameSpear = 1;
              directionSpear = i + 1;
              audio->play_sound({ "./Resources/Sound/SpearSwish.wav" });
              break;
          }
      }
   }
   else if (frameSpear < 18) frameSpear += 3; 
   else frameSpear = 0;
}
void Hero::DrawHero(){ // отрисовка игрока и его копья
   setcolor(RED);
   for(int i = 0; i < 3; i++){
      if(directionSpear == 3){
         moveto(x + 25 + i, y + 36 - frameSpear);
         lineto(x + 25 + i, y + 36 - LenghtSpear - frameSpear);
         if(i == 2){ 
            putimage(x + 22, y + 27 - LenghtSpear - frameSpear, bmp_basic[directionSpear + 4], TRANSPARENT_PUT);
            putimage(x, y, bmp_basic[directionSpear], TRANSPARENT_PUT);
         }
      }
      if(directionSpear == 4){
         moveto(x + 30 - frameSpear, y + 50 + i);
         lineto(x + 30 - LenghtSpear - frameSpear, y + 50 + i);
         if(i == 2){ 
            putimage(x + 21 - LenghtSpear - frameSpear, y + 47, bmp_basic[directionSpear + 4], TRANSPARENT_PUT);
            putimage(x, y, bmp_basic[directionSpear], TRANSPARENT_PUT);
         }
      }
   }
   for(int i = 0; i < 3; i++){
      if(directionSpear == 1){
         if(i == 0){ 
            putimage(x, y + 37 + LenghtSpear + frameSpear, bmp_basic[directionSpear + 4], TRANSPARENT_PUT);
            putimage(x, y, bmp_basic[directionSpear], TRANSPARENT_PUT);
         }
         moveto(x + 3 + i, y + frameSpear + 36);
         lineto(x + 3 + i, y + 36 + LenghtSpear + frameSpear);
      }
      if(directionSpear == 2){
          if(i == 0){ 
            putimage(x + LenghtSpear + frameSpear + 1, y + 47, bmp_basic[directionSpear + 4], TRANSPARENT_PUT);
            putimage(x, y, bmp_basic[directionSpear], TRANSPARENT_PUT);
          }
         moveto(x + frameSpear, y + 50 + i);
         lineto(x + LenghtSpear + frameSpear, y + 50 + i);
      }
   }
}
void Hero::DrawStats(){ // отрисовка характеристик игрока
   settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);   
   setcolor(BLACK);
   setbkcolor(WHITE);
   DrawStat(735, 125, HP); DrawStat(735, 180, Speed/2); DrawStat(735, 225, Damage/2); DrawStat(735, 275, LenghtSpear/5);
   setbkcolor(BLACK);
}

void Hero::DrawItems(){ // отрисовка иконок предметов игрока
   std::array<std::pair<int, int>, 4> item_coords{{ {656, 313}, {733, 313}, {656, 396}, {733, 396} }};

   for (int i = 0; i < nextItem; i++){
      putimage(item_coords[i].first, item_coords[i].second, Items[i], TRANSPARENT_PUT);
   }
}

void DrawStat(int x, int y, int stat){ // отрисовка одной характеристики
   outtextxy(x, y, to_string(stat).c_str());
}
