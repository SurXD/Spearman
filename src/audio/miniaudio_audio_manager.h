#pragma once
#include<stdexcept>
#include "miniaudio.h"
#include "audio_manager.h"

class miniaudio_audio_manager : public audio_manager { //тупое название
	ma_engine engine;
public:
	miniaudio_audio_manager();
	void play_sound(const sound s) override;
	~miniaudio_audio_manager() override;
};
