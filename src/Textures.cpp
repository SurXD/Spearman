#include "textures.h"
#include "game.h"

extern Game* g;

extern HWND hwnd;

button but[COUNT_BUTTON_MENU];

IMAGE* about_screen; //Изображение "О программе"
IMAGE* rules_screen; //Изображение "Правила"
IMAGE* screensaver; //Изображение "Заставка"
IMAGE* main_screen; //Изображение "Меню игры"

IMAGE* bmp_basic[COUNT_ENVIROMENT_OBJECTS]; // массив для различных базовых изображений 
IMAGE* bmp_items[COUNT_ITEMS_SPRITES]; // массив для изображения предметов и иконок предметов
IMAGE* bmp_enemy[ENEMY_TYPE_COUNT]; // массив для изображения врагов 

void load_menu_sprites() // инициализация кнопок меню
{
    screensaver = loadBMP("./Resources/Textures/Spearmanz.bmp");
    main_screen = loadBMP("./Resources/Textures/Spearman.bmp");
    about_screen = loadBMP("./Resources/Textures/rules.bmp");
    rules_screen = loadBMP("./Resources/Textures/about.bmp");

   char s[40];
   for(int i = 0; i < COUNT_BUTTON_MENU; i++)
   {
        but[i].dx = 100; but[i].dy = 50; 
        but[i].x = 100; but[i].y = 230 + i * (but[i].dy+10);   
        sprintf(s,"./Resources/Textures/menu%d.bmp", i+1);
        but[i].bmp = loadBMP(s);
   }
}

void load_items_sprites()
{
    char s[40];
    for (int i = 0; i < COUNT_ENVIROMENT_OBJECTS; i++) {
        sprintf(s, "./Resources/Textures/pic%d.bmp", i);
        bmp_basic[i] = loadBMP(s);
    }
    for (int i = 0; i < COUNT_ITEMS_SPRITES; i++) {
        sprintf(s, "./Resources/Textures/Item%d.bmp", i);
        bmp_items[i] = loadBMP(s);
    }
    res->load_sprite("hero_front", "./Resources/Textures/hero_front.bmp");
    res->load_sprite("hero_back", "./Resources/Textures/hero_back.bmp");
    res->load_sprite("hero_side", "./Resources/Textures/hero_side.bmp");
    res->load_sprite("spear", "./Resources/Textures/spear.bmp");
}

void load_enemy_sprites()
{
    char s[40];
    for (int i = 0; i < ENEMY_TYPE_COUNT; i++) {
        sprintf(s, "./Resources/Textures/enemy%d.bmp", i);
        bmp_enemy[i] = loadBMP(s);
    }
}

int selected{};

void draw_menu() // отрисовка кнопок меню
{
   putimage(0, 0, main_screen, COPY_PUT);

   for(int i = 0; i < COUNT_BUTTON_MENU; i++)
   {
        putimage(but[i].x , but[i].y, but[i].bmp, COPY_PUT);
   }
   setcolor(WHITE);
   setlinestyle(SOLID_LINE, 0, 4);
   rectangle(but[selected].x, but[selected].y, but[selected].x + but[selected].dx, but[selected].y + but[0].dy);
   setlinestyle(SOLID_LINE, 0, 1);
}

void menu() // выбор пункта меню
{
    if (input->is_pressed(key::W) && selected != 0)
    {
        --selected;
    }
    if (input->is_pressed(key::S) && selected != COUNT_BUTTON_MENU -1)
    {
        ++selected;
    }
    if (input->is_pressed(key::E))
    {
        game_state = static_cast<state>(selected + 2);//плохое смещение
        if (static_cast<state>(selected + 2) == state::RUNNING) g->start();
    }
    
    
    int x = 0, y = 0;

    if(mousebuttons() == 1)
    {
        x = mousex();
        y = mousey();
    }

    for (int i = 0; i < COUNT_BUTTON_MENU; ++i)
    {
        if (x >= but[i].x && x <= but[i].x + but[i].dx &&
            y >= but[i].y && y <= but[i].y + but[i].dy)
        {
            game_state = static_cast<state>(i + 2);//плохое смещение
            if (static_cast<state>(i + 2) == state::RUNNING) g->start();
        }
    }
}

void game_over()
{
    if(input->is_pressed(key::Q)) game_state = state::MAIN_MENU;
}

void win()
{
    if(input->is_pressed(key::Q)) game_state = state::MAIN_MENU;
}

void draw_game_over()
{
    putimage(0, 0, bmp_basic[18], COPY_PUT);
}

void draw_win()
{
    putimage(0, 0, bmp_basic[19], COPY_PUT);
}
