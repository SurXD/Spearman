#include"Textures.h"

extern HWND hwnd;
constexpr int BUTTON_MENU_COUNT = 4;
button but[BUTTON_MENU_COUNT];

void init_menu() // инициализация кнопок меню
{
   char s[40];
   for(int i = 0; i < BUTTON_MENU_COUNT; i++)
   {
        but[i].dx = 100; but[i].dy = 50; 
        but[i].x = 100; but[i].y = 230 + i * (but[i].dy+10);   
        sprintf(s,"./Resources/Textures/menu%d.bmp", i+1);
        but[i].bmp = loadBMP(s);
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