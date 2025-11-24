#include "sprite_factory.h"

sprite* sprite_factory::create_sprite(rendering_type backend)
{
    if(backend == rendering_type::WINBGIM) return new winbgim_sprite();
    throw std::runtime_error("rendering backend" + std::to_string(backend) +" is not supported");
}
