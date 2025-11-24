#pragma once

#include "./audio/sound.h"
#include "./rendering/sprite.h"
#include "./rendering/sprite_factory.h"
#include <unordered_map>
#include <string>
#include <iostream>

class resource_manager
{
    rendering_type backend;
    std::unordered_map<std::string, sound*> sounds;
    std::unordered_map<std::string, sprite*> sprites;
public:
    resource_manager(rendering_type backend);
    void load_sound(const std::string& name, const std::string& path);
    void load_sprite(const std::string& name, const std::string& path);

    sound* get_sound(const std::string& name) const { return sounds.at(name); }
    sprite* get_sprite(const std::string& name) const { return sprites.at(name); }
};

