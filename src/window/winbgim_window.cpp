#include "winbgim_window.h"

void winbgim_window::create(const glm::ivec2 size, const std::string& name)
{
    initwindow(size.x, size.y, name.c_str(), 0, 0, true);
}

void winbgim_window::clear()
{
    clearviewport();
}

void winbgim_window::swap_buffers()
{
    swapbuffers();
}

winbgim_window::~winbgim_window()
{
    closegraph();
}
