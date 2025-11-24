#ifndef SPRITE_FACTORY_H
#define SPRITE_FACTORY_H

#include "rendering_type.h"
#include "winbgim_sprite.h"

class sprite_factory
{
    sprite_factory() = delete;
    sprite_factory(const sprite_factory&) = delete;
    sprite_factory operator =(const sprite_factory&) = delete;
    ~sprite_factory(){}
public:
    static sprite* create_sprite(rendering_type backend);
};

#endif // SPRITE_FACTORY_H
