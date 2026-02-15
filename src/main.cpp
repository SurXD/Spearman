#include "textures.h"
#include "audio/miniaudio_audio_manager.h"
#include "input/winapi_input_manager.h"
#include "resource_manager/resource_manager.h"
#include "window/winbgim_window.h"
#include "rendering/winbgim_renderer.h"
#include "game.h"
#include <chrono>

HWND hwnd;//текущее окно

audio_manager* audio = new miniaudio_audio_manager;
input_manager* input = new winapi_input_manager;
resource_manager* res = new resource_manager(rendering_type::WINBGIM);
renderer* r = new winbgim_renderer;
window* w = new winbgim_window;

int SCREEN_WIDTH = 810; // +10 т.к окно почему-то съедает 10 пикселей у картинки с обеих сторон
int SCREEN_HEIGHT = 490; //
int OFFSET_X = 100;
int OFFSET_Y = 100;
//if(input->is_pressed(key::W)) selected--; условно
constexpr double FRAME_TIME = 1.0f / 24.3902439;
state  game_state;
Game* g;

void logic();
void draw();
void init(); // инициализаци¤ программы
void draw_screensaver(); // заставка игры
//void game(); // игра
void about(); // об игре  
void rules(); // управление и правила
void draw_rules_screen();
void pause();
void screensaver_logic();
void draw_pause();
void draw_about_screen();

void logic()
{
    if(game_state == state::SCREENSAVER)
    {
        screensaver_logic();
    }
    if(game_state == state::MAIN_MENU)
    {
        menu();
    }
    if(game_state == state::ABOUT_SCREEN)
    {
        about();
    }
    if(game_state == state::RULES_SCREEN)
    {
        rules();
    }
    if(game_state == state::RUNNING)
    {
        g->logic();
    }
    if(game_state == state::PAUSE)
    {
        pause();
    }
    if(game_state == state::GAME_OVER)
    {
        game_over();
    }
    if(game_state == state::WIN)
    {
        win();
    }
}

void draw()
{
    if(game_state == state::SCREENSAVER)
    {
        draw_screensaver();
    }
    if(game_state == state::MAIN_MENU)
    {
        draw_menu();
    }
    if(game_state == state::ABOUT_SCREEN)
    {
        draw_about_screen();
    }
    if(game_state == state::RULES_SCREEN)
    {
        draw_rules_screen();
    }
    if(game_state == state::RUNNING)
    {
        g->draw();
    }
    if(game_state == state::PAUSE)
    {
        draw_pause();
    }
    if(game_state == state::GAME_OVER)
    {
        draw_game_over();
    }
    if(game_state == state::WIN)
    {
        draw_win();
    }
}

int main() { // основна¤ функци¤
   init();
   hwnd = FindWindow(nullptr, "Spearman");

   g = new Game;

   auto current_time = std::chrono::steady_clock::now();

   double accumulator = 0;


   while(game_state != state::EXIT) {
       w->clear();
       input->pull_events();

       while(accumulator >= FRAME_TIME) {
           logic();
           accumulator -= FRAME_TIME;
       }
       draw();

       w->swap_buffers();

       auto end_time = std::chrono::steady_clock::now();
       double time_passed = std::chrono::duration<double>(end_time - current_time).count();

       current_time = end_time;
       if(time_passed >= 25.0f) time_passed = 25.0f;
       accumulator += time_passed;
   }
   return 0;
}

void init() { // инициализаци¤ программы
    game_state = state::SCREENSAVER;
    w->create({SCREEN_WIDTH, SCREEN_HEIGHT}, "Spearman");
   //initwindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Spearman", OFFSET_X, OFFSET_Y, true);
   load_menu_sprites(); // инициализаци¤ кнопок меню
   load_items_sprites();
   load_enemy_sprites();

   for (int i = 0; i < kinds_of_enemies.size(); ++i) {
       kinds_of_enemies[i].set_sprite(bmp_enemy[i]);
   }   
}
void draw_screensaver() { // заставка игры
   putimage(0, 0, screensaver, COPY_PUT);
}

void screensaver_logic() {
    for(size_t i = 0; i < KEYS_COUNT; i++) {
        key k = static_cast<key>(i);
        if(input->is_pressed(k)) game_state = state::MAIN_MENU;
    }
}

void about() { // об игре 

    if(input->is_pressed(key::Q)) game_state = state::MAIN_MENU;
}

void draw_about_screen() {
    putimage(0, 0, about_screen, COPY_PUT);
}

void rules() { // управление
   if(input->is_pressed(key::Q)) game_state = state::MAIN_MENU;
}

void draw_rules_screen() {
    putimage(0, 0, rules_screen, COPY_PUT);
}

void draw_pause() {
    //std::string info = "Этаж: " + std::to_string(current_floor);
    setcolor(RED);
    outtextxy(700 / 2, 450 /2, "пауза");
    //outtextxy(700 / 2, 450 /2 + 30, info.c_str());
}

void pause() {
    if(input->is_pressed(key::P)) game_state = state::RUNNING;
}

/*void init_set_of_rooms(Enemy normal[])
{
    //¬раги 1 этажа
    a.enemies_location[0][0] = normal[ZOMBIE];   a.enemies_location[0][1] = normal[ZOMBIE];   a.enemies_location[0][2] = normal[ZOMBIE];                                               a.enemies_count[0] = 3;
    a.enemies_location[1][0] = normal[ZOMBIE];   a.enemies_location[1][1] = normal[ZOMBIE];                                                                                            a.enemies_count[1] = 2;
    a.enemies_location[2][0] = normal[ZOMBIE];   a.enemies_location[2][1] = normal[KNIGHT];   a.enemies_location[2][2] = normal[ZOMBIE];                                               a.enemies_count[2] = 3;
    a.enemies_location[3][0] = normal[BOSS1];                                                                                                                                          a.enemies_count[3] = 1;
    //¬раги 2 этажа
    a.enemies_location[4][0] = normal[KNIGHT];   a.enemies_location[4][1] = normal[KNIGHT];                                                                                            a.enemies_count[4] = 2;
    a.enemies_location[5][0] = normal[ZOMBIE];   a.enemies_location[5][1] = normal[KNIGHT];   a.enemies_location[5][2] = normal[KNIGHT];                                               a.enemies_count[5] = 3;
    a.enemies_location[6][0] = normal[NINJA];    a.enemies_location[6][1] = normal[KNIGHT];                                                                                            a.enemies_count[6] = 2;
    a.enemies_location[7][0] = normal[BOSS2];                                                                                                                                          a.enemies_count[7] = 1;
    //¬раги 3 этажа
    a.enemies_location[8][0] = normal[ZOMBIE];   a.enemies_location[8][1] = normal[ZOMBIE];   a.enemies_location[8][2] = normal[ZOMBIE];  a.enemies_location[8][3] = normal[ZOMBIE];   a.enemies_count[8] = 4;
    a.enemies_location[9][0] = normal[NINJA];    a.enemies_location[9][1] = normal[NINJA];                                                                                             a.enemies_count[9] = 2;
    a.enemies_location[10][0] = normal[KNIGHT];  a.enemies_location[10][1] = normal[KNIGHT];  a.enemies_location[10][2] = normal[NINJA];                                               a.enemies_count[10] = 3;
    a.enemies_location[11][0] = normal[BOSS3];                                                                                                                                         a.enemies_count[11] = 1;
    //¬раги 4 этажа
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
}*/

/*void game() { // игра
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
   if(current_floor == -10) game_state = state::GAME_OVER;
   else game_state = state::WIN;
   //draw_end_game(current_floor);
   //здесь происходил segmentation fault;
}*/

