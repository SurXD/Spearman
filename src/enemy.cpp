#include "enemy.h"

array<Enemy, ENEMY_TYPE_COUNT> kinds_of_enemies =
{ {
    {100,  4, bmp_enemy[ZOMBIE]},
    {200,  6, bmp_enemy[KNIGHT]},
    {50,   8, bmp_enemy[NINJA]},
    {1000, 5, bmp_enemy[BOSS1]},
    {700,  7, bmp_enemy[BOSS2]},
    {200,  9, bmp_enemy[BOSS3]},
    {1000, 9, bmp_enemy[BOSS4]}
} };

void Enemy::move(const int hero_x, const int hero_y) { // движение врагов
    double speed_x, speed_y,
           abs_x, abs_y;
    abs_x = abs(x - hero_x);
    abs_y = abs(y - hero_y);
    if (abs_x == 0 && abs_y == 0) {
        speed_x = 0;
        speed_y = 0;
    }
    else if (abs_x < abs_y) {
        speed_y = speed / sqrt(1 + (abs_x / abs_y) * (abs_x / abs_y));
        speed_x = (abs_x / abs_y) * speed_y;
    }
    else if (abs_x > abs_y) {
        speed_x = speed / sqrt(1 + (abs_y / abs_x) * (abs_y / abs_x));
        speed_y = (abs_y / abs_x) * speed_x;
    }
    else
        speed_x = speed_y = speed * sqrt(2.0) / 2;
    if (x - hero_x < 0) speed_x = -speed_x;
    if (y - hero_y < 0) speed_y = -speed_y;
    x -= speed_x;
    y -= speed_y;
}

void Enemy::receive_damage(const int spearhead_x, const int spearhead_y, const int damage) {
    if (hp > 0 && spearhead_x >= x && spearhead_x <= x + WEIGHT_ENEMY_SPRITE
                       && spearhead_y >= y && spearhead_y <= y + HEIGHT_ENEMY_SPRITE)
        hp -= damage;
}

bool Enemy::is_deal_damage(const int& hero_x, const int& hero_y) {
    if (hp > 0) {
        if (x > hero_x - WEIGHT_ENEMY_HITBOX &&
            x < hero_x + WEIGHT_HERO_HITBOX &&
            y > hero_y - HEIGHT_ENEMY_HITBOX &&
            y < hero_y + HEIGHT_HERO_HITBOX)
            return true;
        else return false;
    }
    else return false;
}

void Enemy::draw_health_bar() {
    int indent_from_sprite = (WEIGHT_HEALTH_BAR - WEIGHT_ENEMY_SPRITE) / 2;
    setcolor(BLACK);
    rectangle(x - indent_from_sprite, y - HEIGHT_HEALTH_BAR - LIFTING_HEALTH_BAR, 
              x + WEIGHT_ENEMY_SPRITE + indent_from_sprite, y - LIFTING_HEALTH_BAR);
    setfillstyle(1, RED);
    bar(x - indent_from_sprite, y - HEIGHT_HEALTH_BAR - LIFTING_HEALTH_BAR,
        x - indent_from_sprite + int((hp / start_hp) * WEIGHT_HEALTH_BAR), y - LIFTING_HEALTH_BAR);
}

void Enemy::draw() {
    //renderer->draw(get_enemy_type(enemies[i]), enemies[i].x, enemies[i].y, enemies[i].bmp, TRANSPARENT_PUT);
    putimage(x, y, sprite, TRANSPARENT_PUT);
    draw_health_bar();
}