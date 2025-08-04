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
            // Fine di un record
            if (matched) {
                // Ricava i campi indirizzo separati
                std::string street = entry["address_lines"];
                std::string zip = entry["zip"];
                std::string city = entry["city"];

                // Crea Musician con i nuovi campi
                Musician m(
                    entry["name"],
                    entry["mobile"],
                    entry["instrument"],
                    entry["email"],
                    street,
                    zip,
                    city,
                    0.0, 0, 0, false, 0  // valori di default per gli altri campi
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

