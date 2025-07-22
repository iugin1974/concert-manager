// ScoreSelectView.cpp
#include "ScoreSelectView.h"
#include "Concert.h"

#include <filesystem>
#include <ncurses.h>
#include <algorithm>

namespace fs = std::filesystem;

std::optional<std::string> ScoreSelectView::show() {
    loadScores();
    clear();
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
            case 10: // Enter
                return availablePaths[currentSelection];
            case 27: // ESC
                return std::nullopt;
        }
    }
}

void ScoreSelectView::loadScores() {
    availablePaths.clear();
    std::string base = Concert::basePathScores;
    if (!fs::exists(base) || !fs::is_directory(base)) return;

    for (const auto& entry : fs::recursive_directory_iterator(base)) {
        if (entry.is_regular_file() && entry.path().extension() == ".pdf") {
            availablePaths.push_back(entry.path().string());
        }
    }
    std::sort(availablePaths.begin(), availablePaths.end());
}

void ScoreSelectView::draw() {
    clear();
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
