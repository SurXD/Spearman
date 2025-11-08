#include"Textures.h"
#include "Stage.h"

void Stage::printMap(){ // отрисовка мини-карты
   for(int i = 0; i < 6; i++){
      for(int j = 0; j < 6; j++){
         if(stagemap[i][j] != 0){
         setfillstyle(1, BLACK);
         bar(670 + 21*i, 25 + 14*j, 681 + 21*i, 18 + 14*j);
         if(j < 5 && stagemap[i][j+1] != 0) bar(674 + 21*i, 32 + 14*j, 677 + 21*i, 25 + 14*j);
         if(i < 5 && stagemap[i + 1][j] != 0) bar(681 + 21*i, 20 + 14*j, 691 + 21*i, 23 + 14*j);
         if(stagemap[i][j] == 4) setfillstyle(1, GREEN);
         else if(stagemap[i][j] == 1 || stagemap[i][j] == 3) setfillstyle(1, RED);
         else if(stagemap[i][j] == 2) setfillstyle(1, YELLOW);
         bar(671 + 21*i, 24 + 14*j, 680 + 21*i, 19 + 14*j);
         if(stagemap[i][j] == 3) putimage(671 + 21*i, 20 + 14*j, bmpBASIC[0], TRANSPARENT_PUT);
         }
      }
   }
   setfillstyle(1, RGB(80, 80, 80)); bar(671 + 21*xi, 24 + 14*yj, 680 + 21*xi, 19 + 14*yj);
}
void Stage::initStage(){ // создание этажа
   ItemID = rand() % 4 + 1;
   for(int i = 0; i < 6; i++){
      for(int j = 0; j < 6; j++){
         stagemap[i][j] = 0; stage[i][j] = 0;
      }
   }
   for(int i = 0; i < 5; i++){
      f[i] = 0;
   }
   int t1S, t2S, t1B, t2B, t1I, t2I, t1IS, t2IS, t1BS, t2BS;
   int fBoss = 0, fItem= 0;
   t1S = rand() % 6; t2S = rand() % 6;
   stage[t1S][t2S] = 4;
   xi = t1S; yj = t2S;
   while(fBoss == 0){
      t1B = rand() % 6; t2B = rand() % 6;
      if(abs(t1B - t1S) + abs(t2B - t2S) > 2){
         stage[t1B][t2B] = 3;
         t1BS = t1B; t2BS = t2B;
         fBoss = 1;
      }
   }
   while(fItem == 0){
      t1I = rand() % 6; t2I = rand() % 6;
      if(abs(t1I - t1S) + abs(t2I - t2S) > 2 && abs(t1I - t1B) > 1 && abs(t2I - t2B) > 1){
         stage[t1I][t2I] = 2;
         t1IS = t1I; t2IS = t2I;
         fItem = 1;
      }
   }
   while(t1S != t1B || t2S != t2B){
      if(rand() % 2 == 0){
         if(t1S > t1B){
            t1B++;
            stage[t1B][t2B] = 1;
         }
         else if(t1B > t1S){
            t1B--;
            stage[t1B][t2B] = 1;
         }
      }
      else{
         if(t2S > t2B){
            t2B++;
            stage[t1B][t2B] = 1;
         }
         else if(t2B > t2S){
            t2B--;
            stage[t1B][t2B] = 1;
         }
      }
   }
   while(t1S != t1I || t2S != t2I){
      if(rand() % 2 == 0){
         if(t1S > t1I){
            t1I++;
            stage[t1I][t2I] = 1;
         }
         else if(t1I > t1S){
            t1I--;
            stage[t1I][t2I] = 1;
         }
      }
      else{
         if(t2S > t2I){
            t2I++;
            stage[t1I][t2I] = 1;
         }
         else if(t2I > t2S){
            t2I--;
            stage[t1I][t2I] = 1;
         }
      }
   }
   stage[t1S][t2S] = 4;
   stage[t1IS][t2IS] = 2;
   stage[t1BS][t2BS] = 3;
}
void Stage::printDoorsAndItems(int x){ // отрисовка дверей и предметов
   stagemap[xi][yj] = stage[xi][yj];
   for(int i = 0; i < 5; i++){
      f[i] = 0;
   }
   if(stage[xi][yj] == 3){ 
      setfillstyle(1, BLACK); bar(313, 155, 342, 227); 
      f[0] = 10;
      if(x == 1){ setfillstyle(1, YELLOW); bar(317, 159, 338, 223); }
   }
   else{
      if(xi > 0){
         if(stage[xi - 1][yj] != 0){ f[4] = 1; stagemap[xi - 1][yj] = stage[xi - 1][yj];}
      }
      if(yj > 0){
         if(stage[xi][yj - 1] != 0){ f[3] = 1; stagemap[xi][yj - 1] = stage[xi][yj - 1];}
      }
      if(xi < 5){
         if(stage[xi + 1][yj] != 0){ f[2] = 1; stagemap[xi + 1][yj] = stage[xi + 1][yj];}
      }
      if(yj < 5){
         if(stage[xi][yj + 1] != 0){ f[1] = 1; stagemap[xi][yj + 1] = stage[xi][yj + 1];}
      }
      if(x == 1){
         if(f[3] == 1) putimage(300, 0, bmpBASIC[15], TRANSPARENT_PUT);
         if(f[1] == 1) putimage(300, 412, bmpBASIC[13], TRANSPARENT_PUT);
         if(f[2] == 1) putimage(592, 213, bmpBASIC[14], TRANSPARENT_PUT);
         if(f[4] == 1) putimage(-1, 213, bmpBASIC[16], TRANSPARENT_PUT);
      }
      else{
         if(f[3] == 1) putimage(300, 0, bmpBASIC[11], TRANSPARENT_PUT);
         if(f[1] == 1) putimage(300, 412, bmpBASIC[9], TRANSPARENT_PUT);
         if(f[2] == 1) putimage(592, 213, bmpBASIC[10], TRANSPARENT_PUT);
         if(f[4] == 1) putimage(-1, 213, bmpBASIC[12], TRANSPARENT_PUT);
      }
   }
   if(stage[xi][yj] == 2){
      if(ItemID != 0) f[0] = 11;
      if (ItemID != 0) putimage(315, 213, bmpITEMS[2*ItemID - 1], TRANSPARENT_PUT);
   }
}
int Stage::StageMove(int x){ // перемещение игрока по этажу и взятие предмета
   if(x == 1){ stage[xi][yj] = 4; yj += 1; return stage[xi][yj]; }
   else if(x == 2){ stage[xi][yj] = 4; xi += 1; return stage[xi][yj]; }
   else if(x == 3){ stage[xi][yj] = 4; yj -= 1; return stage[xi][yj]; }
   else if(x == 4){ stage[xi][yj] = 4; xi -= 1; return stage[xi][yj]; }
   else if (x == 10) {  return 10; }
   else if(x == 11) ItemID = 0;
   return -1;
}