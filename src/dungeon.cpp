#include "dungeon.h"

Dungeon::Dungeon() : current_floor(0), floors(MAX_COUNT_FLOOR) {
	for (int i = 0; i < MAX_COUNT_FLOOR; ++i)
		floors[i].init_floor(i);
}

int Dungeon::change_state() {
    floors[current_floor].check_of_rooms();
    hero.spear_attack();
    hero.move();
    floors[current_floor].move_enemies(hero.get_x(), hero.get_y());
    hero.deal_damage(floors[current_floor].get_enemies());
    hero.receive_damage(floors[current_floor].get_enemies());

    if (hero.get_hp() < 1) return HELL;

    if (floors[current_floor].room_is_open()) {
        if (floors[current_floor].get_room_id() == ITEM_ROOM && floors[current_floor].get_item_id() != 0)
            hero.take_item(floors[current_floor].get_item_id());
        if(floors[current_floor].is_completed(hero.get_x(), hero.get_y())) ++current_floor;
        else floors[current_floor].change_room(hero.get_x(), hero.get_y());
    }

    return current_floor;
}

void Dungeon::draw()
{
    floors[current_floor].draw_map();
    floors[current_floor].draw_doors();
    floors[current_floor].draw_items();
    floors[current_floor].draw_stairs();
    hero.draw();
    floors[current_floor].draw_enemies();
    
}