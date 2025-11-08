#include"Textures.h"
#include"Room.h"
#include"Hero.h"
#include"Stage.h"

int b = 1; // этаж
IMAGE * bmpBASIC[20]; // массив для различных базовых изображений 
IMAGE * bmpITEMS[9]; // массив для изображения предметов и иконок предметов
IMAGE * bmpEnemy[7]; // массив для изображения врагов 
set_of_rooms a; // пул комнат

HWND hwnd;//текущее окно

void init(); // инициализация программы
void zastavka(); // заставка игры
void game(); // игра
void about(); // об игре  
void rules(); // управление и правила
bool pause();

int main()
{ // основная функция
   
   init();
   zastavka();
   hwnd = FindWindow(nullptr, "Spearman");

   int st = 0;

   do
   {
      st = menu();
      if(st == 1) game(); 
      else if(st == 2) about(); 
      else if(st == 3) rules();     
   } while(st != 4);
   closegraph();
   return 0;
}

void init(){ // инициализация программы
   initwindow(800, 480, "Spearman", 100, 100);
   init_menu(); // инициализация кнопок меню
}
void zastavka(){ // заставка игры
   IMAGE * bmp1 = loadBMP("./Resources/Textures/Spearmanz.bmp");
   putimage(0, 0, bmp1, COPY_PUT);
   getch();
   freeimage(bmp1);
}

void about(){ // об игре 
   clearviewport();
   IMAGE * bmp1 = loadBMP("./Resources/Textures/about.bmp");
   putimage(0, 0, bmp1, COPY_PUT);
   swapbuffers();
   int f1 = 0;
   while(f1 == 0){
      if ((GetAsyncKeyState('Q') & 0x8000 )&& 
         GetForegroundWindow() == hwnd) f1 = 1;
   }
   freeimage(bmp1);
}

void rules(){ // управление
   clearviewport();
   IMAGE * bmp1 = loadBMP("./Resources/Textures/rules.bmp");
   putimage(0, 0, bmp1, COPY_PUT);
   swapbuffers();
   int f1 = 0;
   while(f1 == 0){
      if ((GetAsyncKeyState('Q') & 0x8000)&& 
         GetForegroundWindow() == hwnd) f1 = 1;
   }
   freeimage(bmp1);
}

void init_set_of_rooms(Enemy normal[])
{
    //Враги 1 этажа
    a.enemies_location[0][0] = normal[ZOMBIE];   a.enemies_location[0][1] = normal[ZOMBIE];   a.enemies_location[0][2] = normal[ZOMBIE];                                               a.enemies_count[0] = 3;
    a.enemies_location[1][0] = normal[ZOMBIE];   a.enemies_location[1][1] = normal[ZOMBIE];                                                                                            a.enemies_count[1] = 2;
    a.enemies_location[2][0] = normal[ZOMBIE];   a.enemies_location[2][1] = normal[KNIGHT];   a.enemies_location[2][2] = normal[ZOMBIE];                                               a.enemies_count[2] = 3;
    a.enemies_location[3][0] = normal[BOSS1];                                                                                                                                          a.enemies_count[3] = 1;
    //Враги 2 этажа
    a.enemies_location[4][0] = normal[KNIGHT];   a.enemies_location[4][1] = normal[KNIGHT];                                                                                            a.enemies_count[4] = 2;
    a.enemies_location[5][0] = normal[ZOMBIE];   a.enemies_location[5][1] = normal[KNIGHT];   a.enemies_location[5][2] = normal[KNIGHT];                                               a.enemies_count[5] = 3;
    a.enemies_location[6][0] = normal[NINJA];    a.enemies_location[6][1] = normal[KNIGHT];                                                                                            a.enemies_count[6] = 2;
    a.enemies_location[7][0] = normal[BOSS2];                                                                                                                                          a.enemies_count[7] = 1;
    //Враги 3 этажа
    a.enemies_location[8][0] = normal[ZOMBIE];   a.enemies_location[8][1] = normal[ZOMBIE];   a.enemies_location[8][2] = normal[ZOMBIE];  a.enemies_location[8][3] = normal[ZOMBIE];   a.enemies_count[8] = 4;
    a.enemies_location[9][0] = normal[NINJA];    a.enemies_location[9][1] = normal[NINJA];                                                                                             a.enemies_count[9] = 2;
    a.enemies_location[10][0] = normal[KNIGHT];  a.enemies_location[10][1] = normal[KNIGHT];  a.enemies_location[10][2] = normal[NINJA];                                               a.enemies_count[10] = 3;
    a.enemies_location[11][0] = normal[BOSS3];                                                                                                                                         a.enemies_count[11] = 1;
    //Враги 4 этажа
    a.enemies_location[12][0] = normal[KNIGHT];  a.enemies_location[12][1] = normal[KNIGHT];  a.enemies_location[12][2] = normal[KNIGHT]; a.enemies_location[12][3] = normal[KNIGHT];  a.enemies_count[12] = 4;
    a.enemies_location[13][0] = normal[KNIGHT];  a.enemies_location[13][1] = normal[NINJA];   a.enemies_location[13][2] = normal[NINJA];                                               a.enemies_count[13] = 3;
    a.enemies_location[14][0] = normal[ZOMBIE];  a.enemies_location[14][1] = normal[KNIGHT];  a.enemies_location[14][2] = normal[NINJA];                                               a.enemies_count[14] = 3;
    a.enemies_location[15][0] = normal[BOSS4];                                                                                                                                         a.enemies_count[15] = 1;
   
    a.enemies_location[0][0].x = 258;   a.enemies_location[0][2].x = 338;
    a.enemies_location[1][0].x = 570;   a.enemies_location[1][0].y = 0;     a.enemies_location[1][1].x = 57;   a.enemies_location[1][1].y = 340;
    a.enemies_location[2][0].x = 258;   a.enemies_location[2][2].x = 338;
    
    a.enemies_location[4][0].x = 258;   a.enemies_location[4][1].x = 338;
    a.enemies_location[5][1].x = 570;   a.enemies_location[5][1].y = 0;     a.enemies_location[5][2].x = 57;   a.enemies_location[5][2].y = 340;
    a.enemies_location[6][0].x = 338;
   
    a.enemies_location[8][0].x = 570;   a.enemies_location[8][0].y = 0;     a.enemies_location[8][1].x = 57;   a.enemies_location[8][1].y = 340;   a.enemies_location[8][2].x = 570;   a.enemies_location[8][2].y = 340;   a.enemies_location[8][3].x = 57;   a.enemies_location[8][3].y = 0;
    a.enemies_location[9][0].x = 258;   a.enemies_location[9][0].y = 205;   a.enemies_location[9][1].x = 338;  a.enemies_location[9][1].y = 205;
    a.enemies_location[10][0].x = 258;  a.enemies_location[10][1].x = 338;
   
    a.enemies_location[12][0].x = 570;  a.enemies_location[12][0].y = 0;    a.enemies_location[12][1].x = 57;  a.enemies_location[12][1].y = 340;  a.enemies_location[12][2].x = 570;  a.enemies_location[12][2].y = 340;  a.enemies_location[12][3].x = 57;  a.enemies_location[12][3].y = 0;
    a.enemies_location[13][0].x = 258;  a.enemies_location[13][1].x = 338;
    a.enemies_location[14][0].x = 258;  a.enemies_location[14][1].x = 338;
}

void game(){ // игра
   int stagef;
   b = 1;//почему то не было
   srand(time(0));
   char s[40]{0};
   for(int i = 0; i < 20; i++){
      sprintf(s,"./Resources/Textures/pic%d.bmp", i);
      bmpBASIC[i] = loadBMP(s);
   }
   for(int i = 0; i < 9; i++){
      sprintf(s,"./Resources/Textures/Item%d.bmp", i);
      bmpITEMS[i] = loadBMP(s);
   }
   for(int i = 0; i < 7; i++){
      sprintf(s,"./Resources/Textures/enemy%d.bmp", i);
      bmpEnemy[i] = loadBMP(s);
   }
   Enemy norm[7] = {
      {298, 205, 100, 100, 4, bmpEnemy[0]},//ZOMBIE 
      {298, 205, 200, 200, 6, bmpEnemy[1]}, //KNIGHT
      {298, 205, 50, 50, 8, bmpEnemy[2]}, //NINJA
      {298, 205, 1000, 1000, 5, bmpEnemy[3]}, //BOSS1
      {298, 205, 700, 700, 7, bmpEnemy[4]}, //BOSS2
      {298, 205, 200, 200, 9, bmpEnemy[5]}, //BOSS3
      {298, 205, 1000, 1000, 9, bmpEnemy[6]}}; //BOSS4
   init_set_of_rooms(norm);
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
      if(pause())
      {
         std::string info = "Этаж: " + std::to_string(b);
         setcolor(RED);
         outtextxy(700 / 2, 450 /2, "Пауза");
         outtextxy(700 / 2, 450 /2 + 30, info.c_str());
         swapbuffers(); 
         continue; 
      }
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
   draw_end_game(b);
   //здесь происходил segmentation fault;
}

bool pause()
{
   static bool flag = false;
   static bool key_pressed = false;
   if(GetForegroundWindow() == hwnd && (GetAsyncKeyState('P') & 0x8000))
   {
      if(!key_pressed)
      {
         key_pressed = true;
         flag = flag == false ? true : false;
      }
   }
   else key_pressed = false;
   return flag;
}