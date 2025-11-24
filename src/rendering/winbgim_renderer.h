#pragma once
#include "renderer.h"

class winbgim_renderer : public renderer
{
public:
    winbgim_renderer();
    void init() override;
    void draw_sprite(sprite* s, const glm::vec2 pos, const glm::vec2 size, const float angle) override;
};
