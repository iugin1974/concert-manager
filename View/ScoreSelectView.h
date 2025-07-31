#ifndef SCORESELECTVIEW_H
#define SCORESELECTVIEW_H

#include <string>
#include <vector>
#include <optional>

class ScoreSelectView {
public:
    // Mostra l'interfaccia di selezione delle partiture e ritorna
    // la path selezionata oppure std::nullopt se l'utente annulla.
	std::vector<std::string> show(std::vector<std::string> availablePaths);

private:
    void draw(const std::vector<std::string>& availablePaths, const std::vector<std::string>& selectedFiles);

    int windowHeight = 0;
    int currentSelection = 0;
    int offset = 0;
};

#endif // SCORESELECTVIEW_H
