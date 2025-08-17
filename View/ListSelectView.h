#ifndef SCORESELECTVIEW_H
#define SCORESELECTVIEW_H

#include <string>
#include <vector>
#include <optional>

class ListSelectView {
public:
	ListSelectView(std::string title =
	        "Select item (SPACE: select - ENTER: finish - ESC: cancel):",
	        bool allSelectable = false)
	        : title(std::move(title)), allSelectable(allSelectable) {}
    // Mostra l'interfaccia di selezione delle partiture e ritorna
    // la path selezionata oppure std::nullopt se l'utente annulla.
	std::optional<std::vector<std::string>> show(std::vector<std::string> items);

private:
    void draw(const std::vector<std::string>& availableItems, const std::vector<std::string>& selectedItems);

    int windowHeight = 0;
    int currentSelection = 0;
    int offset = 0;
    std::string title;
    bool allSelectable;

};

#endif // SCORESELECTVIEW_H
