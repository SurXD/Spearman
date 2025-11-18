#pragma once
#include"sound.h"

class audio_manager
{
public:
	audio_manager() = default;
	virtual void play_sound(const sound s) = 0;
	virtual ~audio_manager(){ }
};