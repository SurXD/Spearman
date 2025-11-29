#include "winbgim_renderer.h"

winbgim_renderer::winbgim_renderer()
{

}

void winbgim_renderer::init()
{
    //empty for winbgim
}

void winbgim_renderer::draw_sprite(sprite* s, const glm::vec2 pos, const glm::vec2 size, const float angle, const sprite_mirroring mirroring)
{
    winbgim_sprite* win_sprite = dynamic_cast<winbgim_sprite*>(s);

    IMAGE* resized = imageresize(win_sprite->get_data(), size.x, size.y);
    IMAGE* rotated = imageturn(resized, angle);
    IMAGE* mirrored = rotated;

    if (mirroring == sprite_mirroring::HORIZONTAL) mirrored = imagemirror(rotated, HORIZ_DIR);
    if (mirroring == sprite_mirroring::VERTICAL) mirrored = imagemirror(rotated, VERT_DIR);

    putimage(pos.x, pos.y, mirrored, TRANSPARENT_PUT);

    freeimage(resized);
    freeimage(rotated);
    freeimage(mirrored);
}
