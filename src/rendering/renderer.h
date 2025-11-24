#pragma once
#include "sprite.h"

class renderer
{
public:
    renderer() = default;
    virtual void init() = 0;
    virtual void draw_sprite(sprite* s, const glm::vec2 pos, const glm::vec2 size, const float angle) = 0;
};
