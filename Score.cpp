#include "Score.h"
#include <algorithm>

Score::Score(const std::string& p) : path(p) {}

void Score::setPath(const std::string& p) {
    path = p;
}

const std::string& Score::getPath() const {
    return path;
}

std::string Score::toString() const {
    // Estrae solo il nome del file dalla path
    // Cerca l’ultimo '/' (Linux) o '\\' (Windows)
    size_t pos = path.find_last_of("/\\");
    if (pos == std::string::npos) {
        return path; // non c’è directory, ritorna tutta la stringa
    }
    return path.substr(pos + 1);
}
