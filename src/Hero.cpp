#include "hero.h"

extern audio_manager* audio;
extern input_manager* input;
extern resource_manager* res;
extern renderer* r;

void Hero::take_item(int& item_id) {
    if (CENTER_ROOM_X - WEIGHT_ITEM_SPRITE / 2 > x - WEIGHT_ITEM_SPRITE &&
        CENTER_ROOM_X - WEIGHT_ITEM_SPRITE / 2 < x + WEIGHT_HERO_SPRITE &&
        CENTER_ROOM_Y - HEIGHT_ITEM_SPRITE / 2 > y - HEIGHT_ITEM_SPRITE &&
        CENTER_ROOM_Y - HEIGHT_ITEM_SPRITE / 2 < y + HEIGHT_HERO_SPRITE) {
        if (item_id == HEART) hp += HEART_EFFECT;
        if (item_id == DUMBBELL) damage += DUMBBELL_EFFECT;
        if (item_id == LIGHTNING) speed += LIGHTNING_EFFECT;
        if (item_id == STICK) spear_length += STICK_EFFECT;
        items[next_item] = bmp_items[2 * item_id - 2];
        ++next_item;
        item_id = 0;
    }
}

void Hero::receive_damage(vector<Enemy> &enemies){ // получение урона
   bool hit = false;
   for (auto& i : enemies) if(i.is_deal_damage(x, y)) hit = true;

   if(frame_no_damage > 0) ++frame_no_damage;
   if(frame_no_damage == 0 && hit == true){
      --hp; 
      frame_no_damage = 1;
      audio->play_sound({ "./Resources/Sound/Hit.wav" });
   }
   if(frame_no_damage == MAX_COUNT_FRAME_NO_DAMAGE) frame_no_damage = 0;
}

void Hero::deal_damage(vector<Enemy> &enemies) { // нанесение урона врагам
   if(frame_spear == 19) {
      std::array<glm::ivec2, 4> coords{{ 
          {x + 5, y + 36 + spear_length + frame_spear},
          {x + spear_length + frame_spear, y + 55},
          {x + 27, y + 36 - spear_length - frame_spear},
          {x + 31 - spear_length - frame_spear, y + 55}
      }};

      glm::ivec2 point = coords[direction_spear - 1];

      for (auto& i : enemies) i.receive_damage(point.x, point.y, damage);
   }
}

void Hero::move() { // движение игрока //когда отрисовку выровняем
    if(input->is_pressed(key::W)) {
        y = max(0, y - speed/2);    //  HEIGHT_DOOR, y + HEIGHT_HERO_SPRITE
    }
    if(input->is_pressed(key::A)) {
        x = max(57, x - speed/2); // HEIGHT_DOOR
    }
    if(input->is_pressed(key::S)) {
        y = min(340, y + speed/2); //HIGHT_PLAY_AREA - HEIGHT_DOOR - HEIGHT_HERO_SPRITE
    }
    if(input->is_pressed(key::D)) {
        x = min(570, x + speed/2); //WEIGHT_PLAY_AREA - HEIGHT_DOOR, x + WEIGHT_HERO_SPRITE
    }
}
void Hero::spear_attack(){ // атака игрока
   if(frame_spear == 0) {
      array<key, 4> keykodes{ key::DOWN, key::RIGHT, key::UP, key::LEFT };

      for(int i = 0; i < keykodes.size(); ++i) {
          if(input->is_pressed(keykodes[i])) {
              frame_spear = 1;
              direction_spear = i + 1;
              audio->play_sound({ "./Resources/Sound/SpearSwish.wav" });
              break;
          }
      }
   }
   else if (frame_spear < 18) frame_spear += 3;
   else frame_spear = 0;
}

void Hero::draw_hero()
{
    if(direction_spear == 1) { //направление вниз 
        r->draw_sprite(res->get_sprite("hero_front"), {x, y});
        r->draw_sprite(res->get_sprite("spear"), {x, y + frame_spear + 36}, { spear_length, 9 }, 270);
    }
    if (direction_spear == 2) { //направление вправо
        r->draw_sprite(res->get_sprite("hero_side"), { x, y });
        r->draw_sprite(res->get_sprite("spear"), { x + frame_spear, y + 50 }, { spear_length, 9 });
    }
    if (direction_spear == 3) { //направление вверх
        r->draw_sprite(res->get_sprite("spear"), { x + 22, y + 36 - spear_length - frame_spear }, { spear_length, 9 }, 90);
        r->draw_sprite(res->get_sprite("hero_back"), { x, y });
    }
    if (direction_spear == 4) { //направление влево
        r->draw_sprite(res->get_sprite("spear"), { x + 31 - spear_length - frame_spear, y + 50 }, { spear_length, 9 }, 180);
        r->draw_sprite(res->get_sprite("hero_side"), { x, y }, {}, 0, sprite_mirroring::HORIZONTAL);
    }
}

void Hero::draw_stats(){ // отрисовка характеристик игрока
   settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);   
   setcolor(BLACK);
   setbkcolor(WHITE);

   outtextxy(LEFT_ALIGHMENT, UP_ALIGHMENT, to_string(hp).c_str());
   outtextxy(LEFT_ALIGHMENT, UP_ALIGHMENT + INTERVAL, to_string(speed / 2).c_str());
   outtextxy(LEFT_ALIGHMENT, UP_ALIGHMENT + 2 * INTERVAL, to_string(damage / 2).c_str());
   outtextxy(LEFT_ALIGHMENT, UP_ALIGHMENT + 3 * INTERVAL, to_string(spear_length / 5).c_str());

   setbkcolor(BLACK);
}

void Hero::draw_items(){ // отрисовка иконок предметов игрока
   std::array<std::pair<int, int>, MAX_COUNT_ITEM> item_coords{{ {WEIGHT_PLAY_AREA, INVENTORY_AREA},
                                                    { WEIGHT_PLAY_AREA + SIZE_INVENTORY_SPRITE, INVENTORY_AREA },
                                                    { WEIGHT_PLAY_AREA, INVENTORY_AREA + SIZE_INVENTORY_SPRITE },
                                                    { WEIGHT_PLAY_AREA + SIZE_INVENTORY_SPRITE, INVENTORY_AREA + SIZE_INVENTORY_SPRITE } }};
   for (int i = 0; i < next_item; i++){
      putimage(item_coords[i].first, item_coords[i].second, items[i], TRANSPARENT_PUT);
   }
}

void Hero::draw() {
    draw_stats();
    draw_hero();
    draw_items();
}