#include "winbgim_sprite.h"

void winbgim_sprite::init(const std::string& filename)
{
    data = loadBMP(filename.c_str());

    if(data == nullptr)
    {
        throw std::runtime_error("failed to open " + filename);
    }

    width = imagewidth(data);
    height = imageheight(data);
}
