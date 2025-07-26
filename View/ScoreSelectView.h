#ifndef SCORESELECTVIEW_H
#define SCORESELECTVIEW_H

#include <string>
#include <vector>
#include <optional>

class ScoreSelectView {
public:
    // Mostra l'interfaccia di selezione delle partiture e ritorna
    // la path selezionata oppure std::nullopt se l'utente annulla.
    std::optional<std::string> show();

private:
    void loadScores();
    void draw();

    std::vector<std::string> availablePaths;
    int windowHeight = 0;
    int currentSelection = 0;
    int offset = 0;
};

#endif // SCORESELECTVIEW_H
