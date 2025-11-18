#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio_audio_manager.h"

miniaudio_audio_manager::miniaudio_audio_manager()
{
    ma_result result;

    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) 
    {
        throw std::runtime_error("failed to init miniaudio");
    }
}

void miniaudio_audio_manager::play_sound(const sound s)
{
    ma_result result = ma_engine_play_sound(&engine, s.get_filename().c_str(), NULL);

    if (result != MA_SUCCESS)
    {
        throw std::runtime_error("failed to play sound: " + s.get_filename());
    }
}

miniaudio_audio_manager::~miniaudio_audio_manager()
{

}
