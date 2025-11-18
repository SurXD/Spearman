#include"Textures.h"
#include"Room.h"
#include"Hero.h"
#include"Stage.h"
#include "audio/miniaudio_audio_manager.h"

int current_floor = 1; // этаж
set_of_rooms a; // пул комнат

HWND hwnd;//текущее окно
audio_manager* audio = new miniaudio_audio_manager;

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 480;
int OFFSET_X = 100;
int OFFSET_Y = 100;
//if(input->is_pressed(key::W)) selected--; условно
enum class state{ SCREENSAVER, MAIN_MENU, ABOUT_SCREEN, RULES_SCREEN, RUNNING, PAUSE, GAME_OVER };//сделать классы 

void init(); // инициализация программы
void draw_screensaver(); // заставка игры
void game(); // игра
void about(); // об игре  
void rules(); // управление и правила
bool pause();

int main() 
{ // основная функция
   
   init();
   draw_screensaver();
   hwnd = FindWindow(nullptr, "Spearman");

   int st = 0;

   do
   {
      st = menu();
      if(st == 1) game(); 
      else if(st == 2) about(); 
      else if(st == 3) rules();     
   } while(st != 4);
   return 0;
}

void init(){ // инициализация программы
   initwindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Spearman", OFFSET_X, OFFSET_Y);
   load_menu_sprites(); // инициализация кнопок меню
   load_items_sprites();
   load_enemy_sprites();
}
void draw_screensaver(){ // заставка игры
   putimage(0, 0, screensaver, COPY_PUT);
   getch();
}

void about() // об игре 
{
   clearviewport();
   putimage(0, 0, about_screen, COPY_PUT);
   swapbuffers();

   int f1 = 0;
   while(f1 == 0){
      if ((GetAsyncKeyState('Q') & 0x8000 )&& 
         GetForegroundWindow() == hwnd) f1 = 1;
   }
}

void rules(){ // управление
   clearviewport();
   putimage(0, 0, rules_screen, COPY_PUT);
   swapbuffers();
   int f1 = 0;
   //if(input->is_pressed(key::Q)) state = MAIN_MENU
   while(f1 == 0){
      if ((GetAsyncKeyState('Q') & 0x8000)&& 
         GetForegroundWindow() == hwnd) f1 = 1;
   }
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
   current_floor = 1;//почему то не было
   srand(time(0));
   //char s[40]{0}; а зачем оно тут было?
   Enemy norm[7] = {
      {298, 205, 100, 100, 4, bmp_enemy[ZOMBIE]},//ZOMBIE 
      {298, 205, 200, 200, 6, bmp_enemy[KNIGHT]}, //KNIGHT
      {298, 205, 50, 50, 8, bmp_enemy[NINJA]}, //NINJA
      {298, 205, 1000, 1000, 5, bmp_enemy[BOSS1]}, //BOSS1
      {298, 205, 700, 700, 7, bmp_enemy[BOSS2]}, //BOSS2
      {298, 205, 200, 200, 9, bmp_enemy[BOSS3]}, //BOSS3
      {298, 205, 1000, 1000, 9, bmp_enemy[BOSS4]}}; //BOSS4
   init_set_of_rooms(norm);
   Room Room;
   Hero Hero;
   Stage Stage;
   Stage.initStage();
   Room.initRoom(4, a);
   while(current_floor > 0 && current_floor < 5)
   {
      clearviewport();
      putimage(0, 0, bmp_basic[17], TRANSPARENT_PUT);
      Stage.check_of_rooms();

      Stage.printDoorsAndItems(Room.is_open());
      Hero.DrawStats();
      Hero.DrawHero();
      Hero.DrawItems();
      Room.printEnemys();
      Stage.printMap();

      if(pause())
      {
         std::string info = "Этаж: " + std::to_string(current_floor);
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
      if(Hero.get_HP() < 1) current_floor = -10;
      Room.OpenRoom();
      if(Room.is_open()){
         stagef = Stage.StageMove(Hero.HeroDoor(Stage.get_adj_rooms(), Stage.getItemID(), Stage.getRoom()));
         if(stagef > 0){
            if(stagef == 10){
               current_floor++;
               Stage.initStage();
            }
            if(current_floor < 5){ Room.initRoom(Stage.getRoom(), a); }
         }
      }
      swapbuffers();
   }
   draw_end_game(current_floor);
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