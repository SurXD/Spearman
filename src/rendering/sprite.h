#pragma once
#include <string>

class sprite
{
protected:
    int width, height;
public:
    sprite() : width(0), height(0) { }
    virtual void init(const std::string& filename) = 0;
    int get_width() const { return width; }
    int get_height() const { return height; }
    virtual ~sprite(){ }
};
