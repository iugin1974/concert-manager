#include "MainMenuView.h"
#include <ncurses.h>
#include <string>
#include <cstring>

int MainMenuView::displayLogo(int row) {
    int height, width;
    getmaxyx(stdscr, height, width);

    const char* lines[] = {
        "+==========================================+",
        "|  ____                          _         |",
        "| / ___|___  _ __   ___ ___ _ __| |_       |",
        "|| |   / _ \\| '_ \\ / __/ _ \\ '__| __|      |",
        "|| |__| (_) | | | | (_|  __/ |  | |_       |",
        "| \\____\\___/|_| |_|\\___\\___|_|   \\__|      |",
        "||  \\/  | __ _ _ __   __ _  __ _  ___ _ __ |",
        "|| |\\/| |/ _` | '_ \\ / _` |/ _` |/ _ \\ '__||",
        "|| |  | | (_| | | | | (_| | (_| |  __/ |   |",
        "||_|  |_|\\__,_|_| |_|\\__,_|\\__, |\\___|_|   |",
        "|                          |___/           |",
        "+==========================================+"
    };

    int numLines = sizeof(lines) / sizeof(lines[0]);

    for (int i = 0; i < numLines; ++i) {
        int len = strlen(lines[i]);
        int col = (width - len) / 2;
        mvprintw(row++, col, "%s", lines[i]);
    }

    return row;
}

MainMenuView::Action MainMenuView::show() {
    clear();
    int height, width;
    getmaxyx(stdscr, height, width);

    int row = 2;
    attron(A_BOLD);
    row = displayLogo(row);

    std::string author = "by Eugenio Giovine";
    mvprintw(row++, (width - author.size()) / 2, "%s", author.c_str());
    row += 2;

    std::string menuTitle = "Menu:";
    mvprintw(row++, (width - menuTitle.size()) / 2, "%s", menuTitle.c_str());
    attroff(A_BOLD);

    const char* choices[] = {
        "1. Add Concert",
        "2. Manage Concerts",
        "3. Save",
        "4. Save and exit"
    };

    int n_choices = sizeof(choices) / sizeof(choices[0]);

    for (int i = 0; i < n_choices; ++i) {
        int col = (width - strlen(choices[i])) / 2;
        mvprintw(row + i + 2, col, "%s", choices[i]);
    }

    while (1) {
        int c = getch();
        c = c - 49;
        if (c < 0 || c >= n_choices)
            continue;
        return static_cast<Action>(c);
    }
}
