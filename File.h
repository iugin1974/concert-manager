// File.h
#pragma once
#include "Concert.h"
#include "tinyxml2.h"
#include <vector>
#include <string>

class File {
public:
void saveConcertsToXML(const std::vector<Concert> &concerts);
std::vector<Concert> loadConcertsFromXML();
private:
std::string getSafeText(tinyxml2::XMLElement* elem); 
};
