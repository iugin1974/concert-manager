#ifndef SCORE_H
#define SCORE_H

#include <string>
#include <vector>

class Score {
private:
    std::string path;

public:
    static std::string basePathScores;
    static std::vector<std::string> scoresDir;
    Score() = default;
    explicit Score(const std::string& p);

    void setPath(const std::string& p);
    const std::string& getPath() const;
    const std::string getFullPath() const;

    // Restituisce solo il nome del file (es. "partitura.pdf")
    std::string toString() const;
};

#endif // SCORE_H
