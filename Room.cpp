#include"Textures.h"
#include "Room.h"

void Room::OpenRoom(){  // проверка на открытие дверей в комнат
   open = 1;
   for(int i = 0; i < num; i++){
      if(arr[i].eHP > 0) open = 0;
   }
}
void Room::Moved(int x, int y){ // движение врагов
   for(int i = 0; i < num; i++){
      if(arr[i].eHP > 0){
         double xSpeed, ySpeed;
         double xabs, yabs;
         xabs = abs(x - arr[i].xe); yabs = abs(y - arr[i].ye);
         if (xabs == 0 && yabs == 0){
            xSpeed = 0; ySpeed = 0;
         }
         else if(xabs < yabs){
            ySpeed = arr[i].eSpeed/sqrt(1+ (xabs/yabs)*(xabs/yabs)); xSpeed = (xabs/yabs) * ySpeed;
         }
         else if(yabs < xabs){
            xSpeed = arr[i].eSpeed/sqrt(1+ (yabs/xabs)*(yabs/xabs)); ySpeed = (yabs/xabs) * xSpeed;
         }
         else{
            xSpeed = ySpeed = arr[i].eSpeed * sqrt(2.0)/2;
         }
         if (arr[i].xe - x < 0){xSpeed = -xSpeed;}
         if (arr[i].ye - y < 0){ySpeed = -ySpeed;}
         arr[i].xe -= xSpeed;
         arr[i].ye -= ySpeed;
      }
   }
}
void Room::printEnemys(){ // отрисовка врагов
   for(int i = 0; i < num; i++){
      if(arr[i].eHP > 0) {
         putimage(arr[i].xe, arr[i].ye, arr[i].bmpe, TRANSPARENT_PUT);
         setcolor(BLACK);
         moveto(arr[i].xe - 15, arr[i].ye - 1);
         lineto(arr[i].xe + 46, arr[i].ye - 1);
         lineto(arr[i].xe + 46, arr[i].ye - 16);
         lineto(arr[i].xe - 15, arr[i].ye - 16);
         lineto(arr[i].xe - 15, arr[i].ye - 1);
         setfillstyle(1, RED);
         bar(arr[i].xe - 15, arr[i].ye - 16, arr[i].xe - 15 + int((arr[i].eHP/arr[i].startHP)*61), arr[i].ye - 1); 
      }
   }
}
void Room::initRoom(int x, ES a){ // создание комнаты
   if(x == 4 || x == 2){
      num = 0; open = 1;
      for(int i = 0; i < 4; i++){
      arr[i].xe = 0; arr[i].ye = 0; arr[i].eHP = 0; arr[i].eSpeed = 0;
      }
   }
   else{ 
      int f = rand() % 3;
      if(x == 3) f = 3;
      for(int i = 0; i < a.nums[b*4 + f - 4]; i++){
         arr[i] = a.mas[b*4 + f - 4][i];
         num = a.nums[b*4 + f - 4];
      }
   }
}