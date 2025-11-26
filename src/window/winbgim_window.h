#pragma once
#include "window.h"
#include <graphics.h>

class winbgim_window : public window
{
public:
    winbgim_window() = default;
    void create(const glm::ivec2 size, const std::string& name) override;
    void clear() override;
    void swap_buffers() override;
    ~winbgim_window() override;
};
