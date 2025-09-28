#pragma once

class Stage{
   private:
      int stage[6][6] = {0}; // структура этажа
      int xi, yj; // координаты игрока на этаже
      int stagemap[6][6] = {0}; // мини-карта этажа
      int ItemID; // ID предмета
      int f[4] = {0}; //массив существования соседних комнат
   public:
      int getRoom(){ return stage[xi][yj];} // получить тип комнаты
      int getItemID() {return ItemID;} // получить ID предмета
      int* getF() {return f;} // получить массив существования соседних комнат
      void printMap(); // отрисовка мини-карты
      void initStage(); // создание этажа
      void printDoorsAndItems(int x); // отрисовка дверей и предметов 
      int StageMove(int x); // перемещение игрока по этажу и взятие предмета
}; // класс для этажа