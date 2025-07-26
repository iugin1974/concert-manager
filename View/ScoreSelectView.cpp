// ScoreSelectView.cpp
#include "ScoreSelectView.h"
#include "Concert.h"
#include "logMessage.h"
#include <filesystem>
#include <ncurses.h>
#include <algorithm>

namespace fs = std::filesystem;

std::optional<std::string> ScoreSelectView::show() {
    loadScores();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    getmaxyx(stdscr, windowHeight, std::ignore);

    draw();

    while (true) {
        int ch = getch();
        switch (ch) {
            case KEY_UP:
                if (currentSelection > 0) {
                    --currentSelection;
                    if (currentSelection < offset) --offset;
                }
                draw();
                break;
            case KEY_DOWN:
                if (currentSelection < (int)availablePaths.size() - 1) {
                    ++currentSelection;
                    if (currentSelection >= offset + windowHeight - 2) ++offset;
                }
                draw();
                break;

            case KEY_PPAGE:  // Page Up
                currentSelection -= (windowHeight - 2);
                if (currentSelection < 0) currentSelection = 0;
                if (currentSelection < offset) offset = currentSelection;
                draw();
                break;

            case KEY_NPAGE:  // Page Down
                currentSelection += (windowHeight - 2);
                if (currentSelection > (int)availablePaths.size() - 1)
                    currentSelection = (int)availablePaths.size() - 1;
                if (currentSelection >= offset + windowHeight - 2)
                    offset = currentSelection - (windowHeight - 2) + 1;
                draw();
                break;

            case KEY_HOME:
                currentSelection = 0;
                offset = 0;
                draw();
                break;

            case KEY_END:
                currentSelection = (int)availablePaths.size() - 1;
                offset = (currentSelection >= windowHeight - 2) ? currentSelection - (windowHeight - 2) + 1 : 0;
                draw();
                break;

            case 10: // Enter
                return availablePaths[currentSelection];
            case 27: // ESC
                return std::nullopt;
        }
    }
}


void ScoreSelectView::loadScores() {
    availablePaths.clear();

    if (Score::basePathScores.empty()) {
        LOG_MSG("basePathScores is empty");
        return;
    }

    namespace fs = std::filesystem;

    for (const auto& dir : Score::scoresDir) {
        fs::path fullPath = fs::path(Score::basePathScores) / dir;

        if (!fs::exists(fullPath) || !fs::is_directory(fullPath)) {
            LOG_MSG(fullPath.string() + " doesn't exist or is not a directory");
            continue;
        }

        for (const auto& entry : fs::recursive_directory_iterator(fullPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".pdf") {
                availablePaths.push_back(entry.path().string());
            }
        }
    }

    // Ordina alfabeticamente (lexicograficamente)
    std::sort(availablePaths.begin(), availablePaths.end(),
        [](const std::string& a, const std::string& b) {
            std::string fileA = fs::path(a).filename().string();
            std::string fileB = fs::path(b).filename().string();
            return fileA < fileB;
        });
}



void ScoreSelectView::draw() {
    int maxDisplay = windowHeight - 2;
    mvprintw(0, 0, "Select a score file (ENTER to confirm, ESC to cancel):");

    for (int i = 0; i < maxDisplay && i + offset < (int)availablePaths.size(); ++i) {
        int index = i + offset;
        if (index == currentSelection) attron(A_REVERSE);
        mvprintw(i + 1, 2, "%s", fs::path(availablePaths[index]).filename().string().c_str());
        if (index == currentSelection) attroff(A_REVERSE);
    }
    refresh();
}
