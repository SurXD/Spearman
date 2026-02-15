#pragma once
#include <string>
#include <glm/glm.hpp>

class sprite {
protected:
    //{width; height}
    glm::ivec2 size;
public:
    sprite() : size{0, 0} { }
    virtual void init(const std::string& filename) = 0;
    glm::ivec2 get_size() const { return size; }
    virtual ~sprite(){ }
};
