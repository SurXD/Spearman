#include"Textures.h"
#include "Stage.h"

void Stage::initStage(){ // создание этажа
   ItemID = rand() % MAX_ITEM_COUNT + 1;
   for (auto& i : stage) i.fill(0); //stage.fill({});
   for (auto& i : stagemap) i.fill(0);

   int start_room_x, start_room_y, end_room_x, end_room_y, item_room_x, item_room_y;
   bool is_end = false, is_item = false;

   start_room_x = rand() % MAX_STAGE_SIZE;
   start_room_y = rand() % MAX_STAGE_SIZE;
   xi = start_room_x; yj = start_room_y;

   while (!is_end) {
      end_room_x = rand() % MAX_STAGE_SIZE;
      end_room_y = rand() % MAX_STAGE_SIZE;
      if (abs(end_room_x - start_room_x) + abs(end_room_y - start_room_y) > MAX_DIST_TO_END) {
          is_end = true;
      }
   }
   while (!is_item) {
      item_room_x = rand() % MAX_STAGE_SIZE;
      item_room_y = rand() % MAX_STAGE_SIZE;
      if(abs(item_room_x - start_room_x) + abs(item_room_y - start_room_y) > MAX_DIST_TO_ITEM &&
         abs(item_room_x - end_room_x) > MAX_DIST_FROM_ITEM_TO_END &&
         abs(item_room_y - end_room_y) > MAX_DIST_FROM_ITEM_TO_END) {
         is_item = true;
      }
   }

   create_way(start_room_x, start_room_y, end_room_x, end_room_y);
   create_way(start_room_x, start_room_y, item_room_x, item_room_y);

   stage[start_room_x][start_room_y] = EMPTY_ROOM;
   stage[item_room_x][item_room_y] = ITEM_ROOM;
   stage[end_room_x][end_room_y] = END_ROOM;
}

void Stage::create_way(int x_current, int y_current, const int x_end, const int y_end) // прокладывает дорогу с обычными врагами от одной комнаты до другой
{
    int step_x = x_end - x_current > 0 ? 1 : -1, 
        step_y = y_end - y_current > 0 ? 1 : -1;

    while (x_current != x_end || y_current != y_end) {
        if (rand() % 2 == 0) {
            if (x_current != x_end) {
                x_current += step_x;
                stage[x_current][y_current] = ENEMY_ROOM;
            }
        }
        else {
            if (y_current != y_end) {
                y_current += step_y;
                stage[x_current][y_current] = ENEMY_ROOM;
            }
        }
    }
}

void Stage::check_of_rooms() { // отмечает смежные комнаты
    adjoining_rooms.fill(0);
    if (yj < MAX_STAGE_SIZE - 1) {
        if (stage[xi][yj + 1]) { adjoining_rooms[UP] = true; stagemap[xi][yj + 1] = stage[xi][yj + 1]; }
    }
    if (xi < MAX_STAGE_SIZE - 1) {
        if (stage[xi + 1][yj]) { adjoining_rooms[RIGHT] = true; stagemap[xi + 1][yj] = stage[xi + 1][yj]; }
    }
    if (yj > 0) {
        if (stage[xi][yj - 1]) { adjoining_rooms[DOWN] = true; stagemap[xi][yj - 1] = stage[xi][yj - 1]; }
    }
    if (xi > 0) {
        if (stage[xi - 1][yj]) { adjoining_rooms[LEFT] = true; stagemap[xi - 1][yj] = stage[xi - 1][yj]; }
    }
}

int Stage::StageMove(int direction) { // перемещение игрока по этажу и взятие предмета
    if(direction >= UP && direction <= LEFT) {
        if (stage[xi][yj] != ITEM_ROOM) stage[xi][yj] = EMPTY_ROOM;
        if      (direction == UP)    return stage[xi][++yj];
        else if (direction == RIGHT) return stage[++xi][yj];
        else if (direction == DOWN)  return stage[xi][--yj];
        else if (direction == LEFT)  return stage[--xi][yj];
    }
    else if (direction == 10) { return 10; }
    else if (direction == 11) { ItemID = 0; stage[xi][yj] = EMPTY_ROOM; }
    return -1;
}

void Stage::printMap() { // отрисовка мини-карты
    for (int i = 0; i < MAX_STAGE_SIZE; ++i) {
        for (int j = 0; j < MAX_STAGE_SIZE; ++j) {
            if (stagemap[i][j] != 0) {
                setfillstyle(1, BLACK);
                bar(670 + 21 * i, 25 + 14 * j, 681 + 21 * i, 18 + 14 * j);
                if (j < 5 && stagemap[i][j + 1] != 0) bar(674 + 21 * i, 32 + 14 * j, 677 + 21 * i, 25 + 14 * j);
                if (i < 5 && stagemap[i + 1][j] != 0) bar(681 + 21 * i, 20 + 14 * j, 691 + 21 * i, 23 + 14 * j);
                if (stagemap[i][j] == 4) setfillstyle(1, GREEN);
                else if (stagemap[i][j] == 1 || stagemap[i][j] == 3) setfillstyle(1, RED);
                else if (stagemap[i][j] == 2) setfillstyle(1, YELLOW);
                bar(671 + 21 * i, 24 + 14 * j, 680 + 21 * i, 19 + 14 * j);
                if (stagemap[i][j] == 3) putimage(671 + 21 * i, 20 + 14 * j, bmp_basic[0], TRANSPARENT_PUT);
            }
        }
    }
    setfillstyle(1, RGB(80, 80, 80)); bar(671 + 21 * xi, 24 + 14 * yj, 680 + 21 * xi, 19 + 14 * yj);
}

void Stage::printDoorsAndItems(bool doors){ // отрисовка дверей и предметов
   stagemap[xi][yj] = stage[xi][yj];
   if(stage[xi][yj] == END_ROOM){ 
      setfillstyle(1, BLACK); bar(313, 155, 342, 227); 
      if(doors){ setfillstyle(1, YELLOW); bar(317, 159, 338, 223); }
   }
   else{
      if(doors){
         if (adjoining_rooms[UP]) putimage(300, 412, bmp_basic[13], TRANSPARENT_PUT);
         if (adjoining_rooms[RIGHT]) putimage(592, 213, bmp_basic[14], TRANSPARENT_PUT);
         if (adjoining_rooms[DOWN]) putimage(300, 0, bmp_basic[15], TRANSPARENT_PUT);
         if (adjoining_rooms[LEFT]) putimage(-1, 213, bmp_basic[16], TRANSPARENT_PUT);
      }
      else{
         if (adjoining_rooms[UP]) putimage(300, 412, bmp_basic[9], TRANSPARENT_PUT);
         if (adjoining_rooms[RIGHT]) putimage(592, 213, bmp_basic[10], TRANSPARENT_PUT);
         if (adjoining_rooms[DOWN]) putimage(300, 0, bmp_basic[11], TRANSPARENT_PUT);
         if (adjoining_rooms[LEFT]) putimage(-1, 213, bmp_basic[12], TRANSPARENT_PUT);
      }
   }
   if(stage[xi][yj] == ITEM_ROOM){
      if (ItemID != 0) putimage(315, 213, bmp_items[2*ItemID - 1], TRANSPARENT_PUT);
   }
}