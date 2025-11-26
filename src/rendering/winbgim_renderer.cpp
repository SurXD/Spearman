#include "winbgim_renderer.h"

winbgim_renderer::winbgim_renderer()
{

}

void winbgim_renderer::init()
{
    //empty for winbgim
}

void winbgim_renderer::draw_sprite(sprite* s, const glm::vec2 pos, const glm::vec2 size, const float angle)
{
    winbgim_sprite* win_sprite = dynamic_cast<winbgim_sprite*>(s);

    IMAGE* rotated_sprite = imageturn(win_sprite->get_data(), angle);
    putimage(pos.x, pos.y, rotated_sprite, COPY_PUT, size.x, size.y);

    freeimage(rotated_sprite);
}
