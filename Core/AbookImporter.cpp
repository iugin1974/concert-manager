// AbookImporter.cpp
#include "AbookImporter.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <map>
#include <algorithm>

static std::string toLower(const std::string& str) {
    std::string res = str;
    std::transform(res.begin(), res.end(), res.begin(), ::tolower);
    return res;
}

std::optional<Musician> AbookImporter::lookupByName(const std::string& name) {
    const char* home = std::getenv("HOME");
    if (!home) return std::nullopt;

    std::ifstream infile(std::string(home) + "/.abook/addressbook");
    if (!infile) return std::nullopt;

    std::map<std::string, std::string> entry;
    std::string line;
    bool matched = false;

    while (std::getline(infile, line)) {
        if (line.empty()) {
            // End of one entry
            if (matched) {
                std::string addr = entry["address_lines"];
                if (!entry["zip"].empty()) addr += ", " + entry["zip"];
                if (!entry["city"].empty()) addr += " " + entry["city"];
                if (!entry["country"].empty()) addr += " (" + entry["country"] + ")";

                Musician m(
                    entry["name"],
                    entry["mobile"],
                    entry["instrument"],
                    entry["email"],
                    addr,
                    0.0, 0, 0, false, 0
                );
                return m;
            }
            entry.clear();
            matched = false;
        } else {
            auto pos = line.find('=');
            if (pos == std::string::npos) continue;
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            entry[key] = value;
            if (key == "name" && toLower(value) == toLower(name)) {
                matched = true;
            }
        }
    }

    return std::nullopt;
}
