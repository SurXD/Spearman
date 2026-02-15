#include "resource_manager.h"

resource_manager::resource_manager(rendering_type backend) : backend(backend) {}

void resource_manager::load_sound(const std::string& name, const std::string& path) {
    //TODO перенести загрузку звука из audio_manager сюда
    sounds[name] = new sound(path);
}

void resource_manager::load_sprite(const std::string& name, const std::string& path) {
    try {
        auto* s = sprite_factory::create_sprite(backend);
        s->init(path);
        sprites[name] = s;
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
