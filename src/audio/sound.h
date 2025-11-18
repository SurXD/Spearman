#pragma once
#include<string>

class sound
{
	const std::string& filename;
public:
	sound(const std::string& filename) : filename(filename){ }
	const std::string& get_filename() const { return filename; }
};