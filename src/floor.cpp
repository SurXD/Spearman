#include "floor.h"

void Floor::init_floor(const int n) { // создание этажа
    number = n;
    item_id = rand() % MAX_COUNT_ITEM + 1;
    for (auto& i : rooms_id) i.fill(ROOM_NOT_EXIST); //rooms_id.fill({});
    for (auto& i : floor_map) i.fill(ROOM_NOT_EXIST);

    int start_room_x, start_room_y, end_room_x, end_room_y, item_room_x, item_room_y;
    bool is_end = false, is_item = false;

    start_room_x = rand() % MAX_SIZE_FLOOR;
    start_room_y = rand() % MAX_SIZE_FLOOR;

    while (!is_end) {
        end_room_x = rand() % MAX_SIZE_FLOOR;
        end_room_y = rand() % MAX_SIZE_FLOOR;
        if (abs(end_room_x - start_room_x) + abs(end_room_y - start_room_y) > MAX_DIST_TO_END) {
            is_end = true;
        }
    }
    while (!is_item) {
        item_room_x = rand() % MAX_SIZE_FLOOR;
        item_room_y = rand() % MAX_SIZE_FLOOR;
        if (abs(item_room_x - start_room_x) + abs(item_room_y - start_room_y) > MAX_DIST_TO_ITEM &&
            abs(item_room_x - end_room_x) > MAX_DIST_FROM_ITEM_TO_END &&
            abs(item_room_y - end_room_y) > MAX_DIST_FROM_ITEM_TO_END) {
            is_item = true;
        }
    }

    create_way(start_room_x, start_room_y, end_room_x, end_room_y);
    create_way(start_room_x, start_room_y, item_room_x, item_room_y);

    rooms_id[start_room_x][start_room_y] = START_ROOM;
    rooms[start_room_x][start_room_y].init_room(START_ROOM, number);
    rooms_id[item_room_x][item_room_y] = ITEM_ROOM;
    rooms[item_room_x][item_room_y].init_room(ITEM_ROOM, number);
    rooms_id[end_room_x][end_room_y] = END_ROOM;
    rooms[end_room_x][end_room_y].init_room(END_ROOM, number);
    floor_map[start_room_x][start_room_y] = START_ROOM;
    xi = start_room_x; yj = start_room_y;
}

void Floor::create_way(int current_x, int current_y, const int end_x, const int end_y) // прокладывает дорогу от одной комнаты до другой
{
    int ENEMY_ROOM;
    int step_x = end_x - current_x > 0 ? 1 : -1,
        step_y = end_y - current_y > 0 ? 1 : -1;

    while (current_x != end_x || current_y != end_y) {
        if (rand() % 2 == 0) {
            if (current_x != end_x) {
                current_x += step_x;
                ENEMY_ROOM = rand() % MAX_COUNT_ENEMY_ROOMS;
                rooms_id[current_x][current_y] = ENEMY_ROOM;
                rooms[current_x][current_y].init_room(ENEMY_ROOM, number);
            }
        }
        else {
            if (current_y != end_y) {
                current_y += step_y;
                ENEMY_ROOM = rand() % MAX_COUNT_ENEMY_ROOMS;
                rooms_id[current_x][current_y] = ENEMY_ROOM;
                rooms[current_x][current_y].init_room(ENEMY_ROOM, number);
            }
        }
    }
}

bool Floor::is_completed(const int& hero_x, const int& hero_y) {
    if (rooms_id[xi][yj] == END_ROOM && is_stairs(hero_x, hero_y)) return true;
    else return false;
}

bool Floor::is_stairs(const int& hero_x, const int& hero_y) {
    if (STAIRS_X > hero_x - WEIGHT_STAIRS &&
        STAIRS_X < hero_x + WEIGHT_HERO_SPRITE &&
        STAIRS_Y > hero_y - HEIGHT_STAIRS &&
        STAIRS_Y < hero_y + HEIGHT_HERO_SPRITE) return true;
    else return false;
}

void Floor::change_room(int& hero_x, int& hero_y) {
    if (rooms_id[xi][yj] != END_ROOM) {
        int direction = get_direction(hero_x, hero_y);
        if (direction != -1) {
            if (direction == DOWN) ++yj;
            else if (direction == RIGHT) ++xi;
            else if (direction == UP) --yj;
            else if (direction == LEFT) --xi;
        }
    }
}

void Floor::check_of_rooms() { // отмечает смежные комнаты
    adjoining_rooms.fill(0);
    if (yj < MAX_SIZE_FLOOR - 1) {
        if (rooms_id[xi][yj + 1] >= 0) { adjoining_rooms[DOWN] = true; floor_map[xi][yj + 1] = rooms_id[xi][yj + 1]; }
    }
    if (xi < MAX_SIZE_FLOOR - 1) {
        if (rooms_id[xi + 1][yj] >= 0) { adjoining_rooms[RIGHT] = true; floor_map[xi + 1][yj] = rooms_id[xi + 1][yj]; }
    }
    if (yj > 0) {
        if (rooms_id[xi][yj - 1] >= 0) { adjoining_rooms[UP] = true; floor_map[xi][yj - 1] = rooms_id[xi][yj - 1]; }
    }
    if (xi > 0) {
        if (rooms_id[xi - 1][yj] >= 0) { adjoining_rooms[LEFT] = true; floor_map[xi - 1][yj] = rooms_id[xi - 1][yj]; }
    }
}

void Floor::draw_map() { // отрисовка мини-карты
    for (int i = 0; i < MAX_SIZE_FLOOR; ++i) {
        for (int j = 0; j < MAX_SIZE_FLOOR; ++j) {
            if (floor_map[i][j] != ROOM_NOT_EXIST) {
                setfillstyle(SOLID_FILL, BLACK);
                bar(MAP_X + (WEIGHT_MAP_ROOM + WEIGHT_TUNNEL) * i,
                    MAP_Y + (HIGHT_MAP_ROOM + HIGHT_TUNNEL) * j,
                    MAP_X + WEIGHT_MAP_ROOM + (WEIGHT_MAP_ROOM + WEIGHT_TUNNEL) * i,
                    MAP_Y + HIGHT_MAP_ROOM + (HIGHT_MAP_ROOM + HIGHT_TUNNEL) * j);

                if (j < MAX_SIZE_FLOOR - 1 && floor_map[i][j + 1] != ROOM_NOT_EXIST)
                    bar(MAP_X + (WEIGHT_MAP_ROOM - THICKNESS_TUNNEL) / 2 + (WEIGHT_MAP_ROOM + WEIGHT_TUNNEL) * i,
                        MAP_Y + HIGHT_MAP_ROOM + (HIGHT_MAP_ROOM + HIGHT_TUNNEL) * j,
                        MAP_X + (WEIGHT_MAP_ROOM - THICKNESS_TUNNEL) / 2 + THICKNESS_TUNNEL + (WEIGHT_MAP_ROOM + WEIGHT_TUNNEL) * i,
                        MAP_Y + HIGHT_MAP_ROOM + HIGHT_TUNNEL + (HIGHT_MAP_ROOM + HIGHT_TUNNEL) * j);

                if (i < MAX_SIZE_FLOOR - 1 && floor_map[i + 1][j] != ROOM_NOT_EXIST)
                    bar(MAP_X + WEIGHT_MAP_ROOM + (WEIGHT_MAP_ROOM + WEIGHT_TUNNEL) * i,
                        MAP_Y + (HIGHT_MAP_ROOM - THICKNESS_TUNNEL) / 2 + (HIGHT_MAP_ROOM + HIGHT_TUNNEL) * j,
                        MAP_X + WEIGHT_MAP_ROOM + WEIGHT_TUNNEL + (WEIGHT_MAP_ROOM + WEIGHT_TUNNEL) * i,
                        MAP_Y + (HIGHT_MAP_ROOM - THICKNESS_TUNNEL) / 2 + THICKNESS_TUNNEL + (HIGHT_MAP_ROOM + HIGHT_TUNNEL) * j);


                if (floor_map[i][j] > ROOM_NOT_EXIST && floor_map[i][j] < START_ROOM && rooms[i][j].is_cleared() ||
                    floor_map[i][j] == START_ROOM || 
                    floor_map[i][j] == ITEM_ROOM && !item_id)
                    setfillstyle(SOLID_FILL, GREEN);
                else if (floor_map[i][j] > ROOM_NOT_EXIST && floor_map[i][j] < START_ROOM) setfillstyle(SOLID_FILL, RED);
                else if (floor_map[i][j] == ITEM_ROOM) setfillstyle(SOLID_FILL, YELLOW);

                bar(MAP_X + MAP_ROOM_JAMB + (WEIGHT_MAP_ROOM + WEIGHT_TUNNEL) * i,
                    MAP_Y + MAP_ROOM_JAMB + (HIGHT_MAP_ROOM + HIGHT_TUNNEL) * j,
                    MAP_X + WEIGHT_MAP_ROOM - MAP_ROOM_JAMB + (WEIGHT_MAP_ROOM + WEIGHT_TUNNEL) * i,
                    MAP_Y + HIGHT_MAP_ROOM - MAP_ROOM_JAMB + (HIGHT_MAP_ROOM + HIGHT_TUNNEL) * j);

                if (floor_map[i][j] == END_ROOM) putimage(MAP_X + MAP_ROOM_JAMB + (WEIGHT_MAP_ROOM + WEIGHT_TUNNEL) * i, MAP_Y + MAP_ROOM_JAMB + (HIGHT_MAP_ROOM + HIGHT_TUNNEL) * j, bmp_basic[0], TRANSPARENT_PUT);
            }
        }
    }
    setfillstyle(SOLID_FILL, HERO_COLOR);

    bar(MAP_X + MAP_ROOM_JAMB + (WEIGHT_MAP_ROOM + WEIGHT_TUNNEL) * xi,
        MAP_Y + MAP_ROOM_JAMB + (HIGHT_MAP_ROOM + HIGHT_TUNNEL) * yj,
        MAP_X + WEIGHT_MAP_ROOM - MAP_ROOM_JAMB + (WEIGHT_MAP_ROOM + WEIGHT_TUNNEL) * xi,
        MAP_Y + HIGHT_MAP_ROOM - MAP_ROOM_JAMB + (HIGHT_MAP_ROOM + HIGHT_TUNNEL) * yj);
}

void Floor::draw_doors(){ // отрисовка дверей
   if(rooms_id[xi][yj] != END_ROOM){
      if(rooms[xi][yj].is_cleared()){
         if (adjoining_rooms[DOWN]) putimage(CENTER_ROOM_X - WEIGHT_DOOR / 2, HIGHT_PLAY_AREA - HEIGHT_DOOR, bmp_basic[13], TRANSPARENT_PUT);
         if (adjoining_rooms[RIGHT]) putimage(WEIGHT_PLAY_AREA - HEIGHT_DOOR, CENTER_ROOM_Y - WEIGHT_DOOR / 2, bmp_basic[14], TRANSPARENT_PUT);
         if (adjoining_rooms[UP]) putimage(CENTER_ROOM_X - WEIGHT_DOOR / 2, 0, bmp_basic[15], TRANSPARENT_PUT);
         if (adjoining_rooms[LEFT]) putimage(-1, CENTER_ROOM_Y - WEIGHT_DOOR / 2, bmp_basic[16], TRANSPARENT_PUT);
      }
      else{
          if (adjoining_rooms[DOWN]) putimage(CENTER_ROOM_X - WEIGHT_DOOR / 2, HIGHT_PLAY_AREA - HEIGHT_DOOR, bmp_basic[9], TRANSPARENT_PUT);
          if (adjoining_rooms[RIGHT]) putimage(WEIGHT_PLAY_AREA - HEIGHT_DOOR, CENTER_ROOM_Y - WEIGHT_DOOR / 2, bmp_basic[10], TRANSPARENT_PUT);
          if (adjoining_rooms[UP]) putimage(CENTER_ROOM_X - WEIGHT_DOOR / 2, 0, bmp_basic[11], TRANSPARENT_PUT);
          if (adjoining_rooms[LEFT]) putimage(-1, CENTER_ROOM_Y - WEIGHT_DOOR / 2, bmp_basic[12], TRANSPARENT_PUT);
      }
   }
}

int Floor::get_direction(int& hero_x, int& hero_y) {
    if (adjoining_rooms[DOWN] && 
        hero_x + WEIGHT_HERO_SPRITE / 2 > CENTER_ROOM_X - WEIGHT_DOOR_GAP / 2 &&
        hero_x + WEIGHT_HERO_SPRITE / 2 < CENTER_ROOM_X - WEIGHT_DOOR_GAP / 2 + WEIGHT_DOOR_GAP &&
        hero_y > HIGHT_PLAY_AREA - HEIGHT_DOOR - HEIGHT_HERO_SPRITE - THICNESS_DOOR_GAP) {
        hero_y = THICNESS_DOOR_GAP + SHIFT_EXIT;
        return DOWN;
    }
    else if (adjoining_rooms[RIGHT] &&
        hero_y + HEIGHT_HERO_SPRITE > CENTER_ROOM_Y - WEIGHT_DOOR_GAP / 2 &&
        hero_y + HEIGHT_HERO_SPRITE < CENTER_ROOM_Y - WEIGHT_DOOR_GAP / 2 + WEIGHT_DOOR_GAP &&
        hero_x > WEIGHT_PLAY_AREA - HEIGHT_DOOR - WEIGHT_HERO_SPRITE - THICNESS_DOOR_GAP) {
        hero_x = HEIGHT_DOOR + THICNESS_DOOR_GAP + SHIFT_EXIT;
        return RIGHT;
    }
    else if (adjoining_rooms[UP] &&
        hero_x + WEIGHT_HERO_SPRITE / 2 > CENTER_ROOM_X - WEIGHT_DOOR_GAP / 2 &&
        hero_x + WEIGHT_HERO_SPRITE / 2 < CENTER_ROOM_X - WEIGHT_DOOR_GAP / 2 + WEIGHT_DOOR_GAP &&
        hero_y < THICNESS_DOOR_GAP) {
        hero_y = HIGHT_PLAY_AREA - HEIGHT_DOOR - HEIGHT_HERO_SPRITE - THICNESS_DOOR_GAP - SHIFT_EXIT;
        return UP;
    }
    else if (adjoining_rooms[LEFT] &&
        hero_y + HEIGHT_HERO_SPRITE > CENTER_ROOM_Y - WEIGHT_DOOR_GAP / 2 &&
        hero_y + HEIGHT_HERO_SPRITE < CENTER_ROOM_Y - WEIGHT_DOOR_GAP / 2 + WEIGHT_DOOR_GAP &&
        hero_x < HEIGHT_DOOR + THICNESS_DOOR_GAP) {
        hero_x = WEIGHT_PLAY_AREA - HEIGHT_DOOR - WEIGHT_HERO_SPRITE - THICNESS_DOOR_GAP - SHIFT_EXIT;
        return LEFT;
    }
    return -1;
}

void Floor::draw_stairs() {
    if (rooms_id[xi][yj] == END_ROOM) {
        setfillstyle(SOLID_FILL, BLACK);
        bar(STAIRS_X, STAIRS_Y, STAIRS_X + WEIGHT_STAIRS, STAIRS_Y + HEIGHT_STAIRS);
        if (rooms[xi][yj].is_cleared()) {
            setfillstyle(SOLID_FILL, YELLOW);
            bar(STAIRS_X + STAIRS_JAMB, STAIRS_Y + STAIRS_JAMB, STAIRS_X + WEIGHT_STAIRS - STAIRS_JAMB, STAIRS_Y + HEIGHT_STAIRS - STAIRS_JAMB);
        }
    }
}

void Floor::draw_items() { // отрисовка предметов
    if (rooms_id[xi][yj] == ITEM_ROOM) {
        if (item_id != 0) putimage(CENTER_ROOM_X - WEIGHT_ITEM_SPRITE / 2, CENTER_ROOM_Y - HEIGHT_ITEM_SPRITE / 2, bmp_items[2 * item_id - 1], TRANSPARENT_PUT);
    }
}