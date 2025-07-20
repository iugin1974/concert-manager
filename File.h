// File.h
#pragma once
#include "Concert.h"
#include "tinyxml2.h"
#include <vector>
#include <string>

class File {
public:
void saveConcertsToXML(const std::vector<Concert> &concerts, const std::string &path) const;
std::vector<Concert> loadConcertsFromXML(const std::string& filename);
private:
std::string getSafeText(tinyxml2::XMLElement* elem); 
};
