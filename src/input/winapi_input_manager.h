#pragma once
#include<array>
#include<Windows.h>
#include "input_manager.h"

class winapi_input_manager : public input_manager
{
	std::array<bool, KEYS_COUNT> keys;

	int key_to_winapi_key(key k);
public:
	winapi_input_manager();
	bool is_pressed(key k) override;
	void pull_events() override;
};
