// ScoreSelectView.cpp
#include "ScoreSelectView.h"
#include "Concert.h"
#include "logMessage.h"
#include <ncurses.h>
#include <filesystem>
#include <algorithm>
#include <optional>

namespace fs = std::filesystem;

std::optional<std::vector<std::string>> ScoreSelectView::show(std::vector<std::string> availablePaths) {
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    getmaxyx(stdscr, windowHeight, std::ignore);

    std::vector<std::string> selectedFiles;

    draw(availablePaths, selectedFiles);

    while (true) {
        int ch = getch();
        if (std::isalpha(ch)) {
            char target = std::tolower(ch);
            for (size_t i = 0; i < availablePaths.size(); ++i) {
                std::string filename = fs::path(availablePaths[i]).filename().string();
                if (!filename.empty() && std::tolower(filename[0]) == target) {
                    currentSelection = static_cast<int>(i);
                    if (currentSelection < offset)
                        offset = currentSelection;
                    else if (currentSelection >= offset + windowHeight - 2)
                        offset = currentSelection - (windowHeight - 2) + 1;
                    draw(availablePaths, selectedFiles);
                    break;
                }
            }
            continue;
        }
        switch (ch) {
            case KEY_UP:
                if (currentSelection > 0) {
                    --currentSelection;
                    if (currentSelection < offset) --offset;
                }
                draw(availablePaths, selectedFiles);
                break;
            case KEY_DOWN:
                if (currentSelection < (int)availablePaths.size() - 1) {
                    ++currentSelection;
                    if (currentSelection >= offset + windowHeight - 2) ++offset;
                }
                draw(availablePaths, selectedFiles);
                break;
            case KEY_PPAGE:
                currentSelection -= (windowHeight - 2);
                if (currentSelection < 0) currentSelection = 0;
                if (currentSelection < offset) offset = currentSelection;
                draw(availablePaths, selectedFiles);
                break;
            case KEY_NPAGE:
                currentSelection += (windowHeight - 2);
                if (currentSelection > (int)availablePaths.size() - 1)
                    currentSelection = (int)availablePaths.size() - 1;
                if (currentSelection >= offset + windowHeight - 2)
                    offset = currentSelection - (windowHeight - 2) + 1;
                draw(availablePaths, selectedFiles);
                break;
            case KEY_HOME:
                currentSelection = 0;
                offset = 0;
                draw(availablePaths, selectedFiles);
                break;
            case KEY_END:
                currentSelection = (int)availablePaths.size() - 1;
                offset = (currentSelection >= windowHeight - 2) ? currentSelection - (windowHeight - 2) + 1 : 0;
                draw(availablePaths, selectedFiles);
                break;
            case ' ':
                {
                    std::string chosen = availablePaths[currentSelection];
                    if (std::find(selectedFiles.begin(), selectedFiles.end(), chosen) == selectedFiles.end()) {
                        selectedFiles.push_back(chosen);
                    }
                    draw(availablePaths, selectedFiles);
                }
                break;
            case 10: // Enter
            	return selectedFiles;
            case 27: // ESC
                return std::nullopt;
        }
    }
}






void ScoreSelectView::draw(const std::vector<std::string>& availablePaths, const std::vector<std::string>& selectedFiles) {
    clear();
    int maxDisplay = windowHeight - 2;
    mvprintw(0, 0, "Select a score file (SPACE: select - ENTER: finish - ESC: cancel):");

    for (int i = 0; i < maxDisplay && i + offset < (int)availablePaths.size(); ++i) {
        int index = i + offset;
        bool isSelected = std::find(selectedFiles.begin(), selectedFiles.end(), availablePaths[index]) != selectedFiles.end();

        if (index == currentSelection) attron(A_REVERSE);

        std::string filename = fs::path(availablePaths[index]).filename().string();

        if (isSelected) {
            filename += " *";  // asterisco per file selezionati
        }

        mvprintw(i + 1, 2, "%s", filename.c_str());

        if (index == currentSelection) attroff(A_REVERSE);
    }
    refresh();
}
