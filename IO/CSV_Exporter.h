#pragma once

#include "Concert.h"
#include <string>

class CSV_Exporter {
public:
	static void exportCSV(const Concert &concert, const std::string &filename);
};
