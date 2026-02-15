#pragma once
#include "sprite.h"

enum class sprite_mirroring{ NONE, HORIZONTAL, VERTICAL };

class renderer {
public:
    renderer() = default;

    virtual void init() = 0;

    virtual void draw_sprite(sprite* s, const glm::vec2 pos, const glm::vec2 size = {}, 
        const float angle = 0, const sprite_mirroring mirroring = sprite_mirroring::NONE) = 0;
};
