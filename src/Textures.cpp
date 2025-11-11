#include"Textures.h"

extern HWND hwnd;

button but[BUTTON_MENU_COUNT];

IMAGE* aboutScreen; //Изображение "О программе"
IMAGE* rulesScreen; //Изображение "Правила"
IMAGE* screensaver; //Изображение "Заставка"

IMAGE* bmpBASIC[ENVIROMENT_OBJECTS_COUNT]; // массив для различных базовых изображений 
IMAGE* bmpITEMS[ITEMS_COUNT]; // массив для изображения предметов и иконок предметов
IMAGE* bmpEnemy[ENEMY_TYPE_COUNT]; // массив для изображения врагов 

void load_menu_sprites() // инициализация кнопок меню
{
    screensaver = loadBMP("./Resources/Textures/Spearmanz.bmp");
    aboutScreen = loadBMP("./Resources/Textures/rules.bmp");
    rulesScreen = loadBMP("./Resources/Textures/about.bmp");

   char s[40];
   for(int i = 0; i < BUTTON_MENU_COUNT; i++)
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
    for (int i = 0; i < ENVIROMENT_OBJECTS_COUNT; i++) {
        sprintf(s, "./Resources/Textures/pic%d.bmp", i);
        bmpBASIC[i] = loadBMP(s);
    }
    for (int i = 0; i < ITEMS_COUNT; i++) {
        sprintf(s, "./Resources/Textures/Item%d.bmp", i);
        bmpITEMS[i] = loadBMP(s);
    }
}

void load_enemy_sprites()
{
    char s[40];
    for (int i = 0; i < ENEMY_TYPE_COUNT; i++) {
        sprintf(s, "./Resources/Textures/enemy%d.bmp", i);
        bmpEnemy[i] = loadBMP(s);
    }
}

void draw_menu() // отрисовка кнопок меню
{
   clearviewport();

   IMAGE * bmp1 = loadBMP("./Resources/Textures/Spearman.bmp");//вынести в загрузку ресурсов
   putimage(0, 0, bmp1, COPY_PUT);

   for(int i = 0; i < BUTTON_MENU_COUNT; i++)
   {
        putimage(but[i].x , but[i].y, but[i].bmp, COPY_PUT);
   }
   freeimage(bmp1);
}

int menu() // выбор пункта меню
{
   int x = 0, y = 0;
   draw_menu();//отрисовка в отдельном блоке
   swapbuffers();

    while(mousebuttons() != 1)
    {
        x = mousex();
        y = mousey();  
    }

    for (int i = 0; i < BUTTON_MENU_COUNT; ++i) 
    {
        if (x >= but[i].x && x <= but[i].x + but[i].dx &&
            y >= but[i].y && y <= but[i].y + but[i].dy)
        {
            return i + 1;
        }
    }
   return 0;
}

void draw_end_game(int x){ // отрисока финального экрана
   if(x < 0) putimage(0, 0, bmpBASIC[18], COPY_PUT);
   else putimage(0, 0, bmpBASIC[19], COPY_PUT);
   swapbuffers();
   int f1 = 0;
   //тут возврат в главное меню
   while(f1 == 0){
      if ((GetAsyncKeyState('Q') & 0x8000) && 
         GetForegroundWindow() == hwnd)
      {
          f1 = 1;/*closegraph(); exit(0);*/
      }
   }
}