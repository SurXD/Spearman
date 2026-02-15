#pragma once
#include"input_types.h"

class input_manager {
public:
	input_manager() = default;
	virtual bool is_pressed(key k) = 0;
	virtual void pull_events() = 0;
};