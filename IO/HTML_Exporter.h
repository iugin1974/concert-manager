#pragma once
#include "Concert.h"

class HTML_Exporter {
public:
	static void saveHTML(const Concert& c, const std::string &filename);

private:
	static std::string toCamelCase(const std::string& input);
};
