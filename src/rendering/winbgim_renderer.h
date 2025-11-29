#pragma once
#include "renderer.h"
#include "winbgim_sprite.h"

class winbgim_renderer : public renderer
{
public:
    winbgim_renderer();
    void init() override;
    void draw_sprite(sprite* s, const glm::vec2 pos, const glm::vec2 size = { }, 
        const float angle = 0, const sprite_mirroring mirroring = sprite_mirroring::NONE) override;
};
