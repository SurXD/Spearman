#include"Textures.h"
#include"Room.h"
#include"Hero.h"
#include"Stage.h"

int b = 1; // этаж
IMAGE * bmpBASIC[20]; // массив для различных базовых изображений 
IMAGE * bmpITEMS[9]; // массив для изображения предметов и иконок предметов
IMAGE * bmpEnemy[7]; // массив для изображения врагов 
ES a; // пул комнат

void init(); // инициализация программы
void zastavka(); // заставка игры
void game(); // игра
void about(); // об игре  
void rules(); // управление и правила

int main(){ // основная функция
   int st = 1;
   init();
   zastavka();
   do{
      st = menu(st);
      if(st == 1) game(); 
      if(st == 2) about(); 
      if(st == 3) rules();         
   } while(st != 4);
   closegraph();
   exit(0);
   return 0;
}

void init(){ // инициализация программы
   initwindow(800, 480, "Spearman", 100, 100);
   init_menu(); // инициализация кнопок меню
}
void zastavka(){ // заставка игры
   IMAGE * bmp1 = loadBMP("Spearmanz.bmp");
   putimage(0, 0, bmp1, COPY_PUT);
   getch();
   freeimage(bmp1);
}

void about(){ // об игре 
   clearviewport();
   IMAGE * bmp1 = loadBMP("about.bmp");
   putimage(0, 0, bmp1, COPY_PUT);
   int f1 = 0;
   while(f1 == 0){
      if (GetAsyncKeyState('Q') & 0x8000) f1 = 1;
   }
   freeimage(bmp1);
}

void rules(){ // управление
   clearviewport();
   IMAGE * bmp1 = loadBMP("rules.bmp");
   putimage(0, 0, bmp1, COPY_PUT);
   int f1 = 0;
   while(f1 == 0){
      if (GetAsyncKeyState('Q') & 0x8000) f1 = 1;
   }
   freeimage(bmp1);
}

void init_ES(Enemy normal[]){
   a.mas[0][0] = normal[0]; a.mas[0][1] = normal[0]; a.mas[0][2] = normal[0]; a.nums[0] = 3;
   a.mas[1][0] = normal[0]; a.mas[1][1] = normal[0]; a.nums[1] = 2;
   a.mas[2][0] = normal[0]; a.mas[2][1] = normal[1]; a.mas[2][2] = normal[0]; a.nums[2] = 3;
   a.mas[3][0] = normal[3]; a.nums[3] = 1;
   a.mas[4][0] = normal[1]; a.mas[4][1] = normal[1]; a.nums[4] = 2;
   a.mas[5][0] = normal[0]; a.mas[5][1] = normal[1]; a.mas[5][2] = normal[1]; a.nums[5] = 3;
   a.mas[6][0] = normal[2]; a.mas[6][1] = normal[1]; a.nums[6] = 2;
   a.mas[7][0] = normal[4]; a.nums[7] = 1;
   a.mas[8][0] = normal[0]; a.mas[8][1] = normal[0]; a.mas[8][2] = normal[0]; a.mas[8][3] = normal[0]; a.nums[8] = 4;
   a.mas[9][0] = normal[2]; a.mas[9][1] = normal[2]; a.nums[9] = 2;
   a.mas[10][0]=normal[1]; a.mas[10][1] = normal[1]; a.mas[10][2] = normal[2]; a.nums[10] = 3;
   a.mas[11][0]=normal[5]; a.nums[11] = 1;
   a.mas[12][0]=normal[1]; a.mas[12][1] = normal[1]; a.mas[12][2] = normal[1]; a.mas[12][3] = normal[1]; a.nums[12] = 4;
   a.mas[13][0]=normal[1]; a.mas[13][1] = normal[2]; a.mas[13][2] = normal[2]; a.nums[13] = 3;
   a.mas[14][0]=normal[0]; a.mas[14][1] = normal[1]; a.mas[14][2] = normal[2]; a.nums[14] = 3;
   a.mas[15][0]=normal[6]; a.nums[15] = 1;
   a.mas[0][0].xe = 258; a.mas[0][2].xe = 338;
   a.mas[1][0].xe = 570; a.mas[1][0].ye = 0; a.mas[1][1].xe = 57; a.mas[1][1].ye = 340;
   a.mas[2][0].xe = 258; a.mas[2][2].xe = 338;
   a.mas[4][0].xe = 258; a.mas[4][1].xe = 338;
   a.mas[5][1].xe = 570; a.mas[5][1].ye = 0; a.mas[5][2].xe = 57; a.mas[5][2].ye = 340; 
   a.mas[6][0].xe = 338;
   a.mas[8][0].xe = 570; a.mas[8][0].ye = 0; a.mas[8][1].xe = 57; a.mas[8][1].ye = 340; a.mas[8][2].xe = 570; a.mas[8][2].ye = 340; a.mas[8][3].xe = 57; a.mas[8][3].ye = 0;
   a.mas[9][0].xe = 258; a.mas[9][0].ye = 205; a.mas[9][1].xe = 338; a.mas[9][1].ye = 205;
   a.mas[10][0].xe = 258; a.mas[10][1].xe = 338;
   a.mas[12][0].xe = 570; a.mas[12][0].ye = 0; a.mas[12][1].xe = 57; a.mas[12][1].ye = 340; a.mas[12][2].xe = 570; a.mas[12][2].ye = 340; a.mas[12][3].xe = 57; a.mas[12][3].ye = 0; 
   a.mas[13][0].xe = 258; a.mas[13][1].xe = 338;
   a.mas[14][0].xe = 258; a.mas[14][1].xe = 338;
}

void game(){ // игра
   int stagef;
   srand(time(0));
   char s[7];
   for(int i = 0; i < 20; i++){
      sprintf(s,"pic%d.bmp", i);
      bmpBASIC[i] = loadBMP(s);
   }
   for(int i = 0; i < 9; i++){
      sprintf(s,"Item%d.bmp", i);
      bmpITEMS[i] = loadBMP(s);
   }
   for(int i = 0; i < 7; i++){
      sprintf(s,"enemy%d.bmp", i);
      bmpEnemy[i] = loadBMP(s);
   }
   Enemy norm[7] = {
      {298, 205, 100, 100, 4, bmpEnemy[0]}, 
      {298, 205, 200, 200, 6, bmpEnemy[1]}, 
      {298, 205, 50, 50, 8, bmpEnemy[2]}, 
      {298, 205, 1000, 1000, 5, bmpEnemy[3]}, 
      {298, 205, 700, 700, 7, bmpEnemy[4]}, 
      {298, 205, 200, 200, 9, bmpEnemy[5]}, 
      {298, 205, 1000, 1000, 9, bmpEnemy[6]}};
   init_ES(norm);
   Room Room;
   Hero Hero;
   Stage Stage;
   Stage.initStage();
   Room.initRoom(4, a);
   while(b > 0 && b < 5){
      clearviewport();
      putimage(0, 0, bmpBASIC[17], TRANSPARENT_PUT);
      Stage.printDoorsAndItems(Room.get_open());
      Hero.DrawStats();
      Hero.DrawHero();
      Hero.DrawItems();
      Room.printEnemys();
      Stage.printMap();
      Hero.SpearAttack();
      Hero.Move();
      Room.Moved(Hero.get_x(), Hero.get_y());
      Hero.DamageEnemy(Room.getArr());
      Hero.DamageMe(Room.getArr(), Room.get_num());
      delay(41);
      Hero.Move();
      if(Hero.get_HP() < 1) b = -10;
      Room.OpenRoom();
      if(Room.get_open() == 1){
         stagef = Stage.StageMove(Hero.HeroDoor(Stage.getF(), Stage.getItemID()));
         if(stagef > 0){
            if(stagef == 10){
               b++;
               Stage.initStage();
            }
            if(b < 5){ Room.initRoom(Stage.getRoom(), a); }
         }
      }
      swapbuffers();
   }
   draw_LoseOrWin(b);
}