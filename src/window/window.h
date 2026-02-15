#pragma once
#include <glm/glm.hpp>
#include <string>

class window {
public:
    window() = default;
    virtual void create(const glm::ivec2 size, const std::string& name) = 0;
    virtual void clear() = 0;
    virtual void swap_buffers() = 0;
    virtual ~window(){}
};
