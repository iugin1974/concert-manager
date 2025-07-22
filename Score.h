#ifndef SCORE_H
#define SCORE_H

#include <string>

class Score {
private:
    std::string path;

public:
    Score() = default;
    explicit Score(const std::string& p);

    void setPath(const std::string& p);
    const std::string& getPath() const;

    // Restituisce solo il nome del file (es. "partitura.pdf")
    std::string toString() const;
};

#endif // SCORE_H
