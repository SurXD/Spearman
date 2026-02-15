#pragma once

#include "sprite.h"
#include <graphics.h>
#include <stdexcept>

class winbgim_sprite : public sprite {
    IMAGE* data;
public:
    winbgim_sprite() : sprite(), data(nullptr) { }

    void init(const std::string& filename) override;
    IMAGE* get_data(){ return data; }

    ~winbgim_sprite(){ if(data != nullptr) freeimage(data); }
};
