// AbookImporter.h
#pragma once
#include <string>
#include <optional>
#include "Musician.h"

class AbookImporter {
public:
    static std::optional<Musician> lookupByName(const std::string& name);
};
