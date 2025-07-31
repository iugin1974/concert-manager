// ScoreSelectView.cpp
#include "ScoreSelectView.h"
#include "Concert.h"
#include "logMessage.h"
#include <ncurses.h>
#include <filesystem>

namespace fs = std::filesystem;

std::optional<std::string> ScoreSelectView::show(std::vector<std::string> availablePaths) {
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    getmaxyx(stdscr, windowHeight, std::ignore);

    draw(availablePaths);

    while (true) {
        int ch = getch();
        if (std::isalpha(ch)) {
                   char target = std::tolower(ch);
                   for (size_t i = 0; i < availablePaths.size(); ++i) {
                       std::string filename = fs::path(availablePaths[i]).filename().string();
                       if (!filename.empty() && std::tolower(filename[0]) == target) {
                           currentSelection = static_cast<int>(i);
                           // Scroll so selection is visible
                           if (currentSelection < offset)
                               offset = currentSelection;
                           else if (currentSelection >= offset + windowHeight - 2)
                               offset = currentSelection - (windowHeight - 2) + 1;
                           draw(availablePaths);
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
                draw(availablePaths);
                break;
            case KEY_DOWN:
                if (currentSelection < (int)availablePaths.size() - 1) {
                    ++currentSelection;
                    if (currentSelection >= offset + windowHeight - 2) ++offset;
                }
                draw(availablePaths);
                break;

            case KEY_PPAGE:  // Page Up
                currentSelection -= (windowHeight - 2);
                if (currentSelection < 0) currentSelection = 0;
                if (currentSelection < offset) offset = currentSelection;
                draw(availablePaths);
                break;

            case KEY_NPAGE:  // Page Down
                currentSelection += (windowHeight - 2);
                if (currentSelection > (int)availablePaths.size() - 1)
                    currentSelection = (int)availablePaths.size() - 1;
                if (currentSelection >= offset + windowHeight - 2)
                    offset = currentSelection - (windowHeight - 2) + 1;
                draw(availablePaths);
                break;

            case KEY_HOME:
                currentSelection = 0;
                offset = 0;
                draw(availablePaths);
                break;

            case KEY_END:
                currentSelection = (int)availablePaths.size() - 1;
                offset = (currentSelection >= windowHeight - 2) ? currentSelection - (windowHeight - 2) + 1 : 0;
                draw(availablePaths);
                break;

            case 10: // Enter
                return availablePaths[currentSelection];
            case 27: // ESC
                return std::nullopt;
        }
    }
}





void ScoreSelectView::draw(std::vector<std::string>& availablePaths) {
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
