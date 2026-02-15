#ifndef ENEMY_H
#define ENEMY_H

#include "textures.h"

class Enemy
{
	int x, y;
	int hp;
	double start_hp;
	int speed;
	IMAGE* sprite;
	void draw_health_bar();
public:
	Enemy(const int hp, const int s, IMAGE* bmp) : x(0), y(0), hp(hp), start_hp(hp), speed(s), sprite(bmp) {}
	int& get_x() { return x; }
	int& get_y() { return y; }
	int& get_hp() { return hp; }
	void set_sprite(IMAGE* s) { sprite = s; }

	void move(const int hero_x, const int hero_y);
    void set_coordinates(pair<int, int> point) { x = point.first; y = point.second; }
    void receive_damage(const int spearhead_x, const int spearhead_y, const int damage);
	bool is_deal_damage(const int &hero_x, const int &hero_y);

	void draw();
}; 

enum { ZOMBIE = 0, KNIGHT, NINJA };
enum { BOSS1 = 3, BOSS2, BOSS3, BOSS4 };

extern array<Enemy, ENEMY_TYPE_COUNT> kinds_of_enemies;

#endif // ENEMY_H