#pragma once
#include "Concert.h"

class HTML {
public:
	void saveHTML(Concert& c);

private:
	std::string toCamelCase(const std::string& input);
};
