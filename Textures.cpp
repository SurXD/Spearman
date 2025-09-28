#include"Textures.h"

Button but[4];

void init_menu(){ // инициализация кнопок меню
   char s[11];
   for(int i = 0; i < 4; i++){
         but[i].dx = 100; but[i].dy = 50; 
         but[i].x = 100; but[i].y = 230 + i * (but[i].dy+10);   
         sprintf(s,"menu%d.bmp", i+1);
         but[i].bmp = loadBMP(s);
   }
}
void draw_menu(){ // отрисовка кнопок меню
   clearviewport();
   IMAGE * bmp1 = loadBMP("Spearman.bmp");
   putimage(0, 0, bmp1, COPY_PUT);
   for(int i = 0; i < 4; i++){
        putimage(but[i].x , but[i].y, but[i].bmp, COPY_PUT);
   }
}
int menu(int st){ // выбор пункта меню
   int flag = 0;
   int x, y;
   draw_menu();
   do {
         while(mousebuttons() != 1) {  x = mousex();    y = mousey();  }
         for (int i=0; i < 4; i++)
               if (x >= but[i].x && x <= but[i].x +but[i].dx &&
                    y >= but[i].y && y <= but[i].y +but[i].dy) 
                  {        
                           flag = 1;
                           st = i+1;
                           break;
                  }
   } while(!flag);
   return st;
}

void draw_LoseOrWin(int x){ // отрисока финального экрана
   if(x < 0) putimage(0, 0, bmpBASIC[18], COPY_PUT);
   else putimage(0, 0, bmpBASIC[19], COPY_PUT);
   swapbuffers();
   int f1 = 0;
   while(f1 == 0){
      if (GetAsyncKeyState('Q') & 0x8000){ f1 = 1; closegraph(); exit(0); }
   }
}