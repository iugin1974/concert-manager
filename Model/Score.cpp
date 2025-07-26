#include "Score.h"
#include <algorithm>
#include <vector>

std::string Score::basePathScores = "";
std::vector<std::string> Score::scoresDir = {};

Score::Score(const std::string& p) : path(p) {}

void Score::setPath(const std::string& p) {
    if (p.rfind(basePathScores, 0) == 0) {
        // Il path inizia con basePathScores → salva solo la parte relativa
        path = p.substr(basePathScores.length());
        // Rimuove eventuale '/' iniziale rimasto
        if (!path.empty() && path[0] == '/')
            path = path.substr(1);
    } else {
        // Path assoluto o da posizione diversa → salva come è
        path = p;
    }
}

const std::string& Score::getPath() const {
    return path;
}

const std::string Score::getFullPath() const {
	return basePathScores + "/" + path;
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
