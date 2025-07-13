#ifndef MENUBAR_H
#define MENUBAR_H

#include <ncurses.h>
#include <vector>
#include <string>

class MenuBar {
public:
    MenuBar(WINDOW *win,
            const std::vector<std::string>& titles,
            const std::vector<std::vector<std::string>>& items);

    // Mostra la menubar, restituisce (menuIndex, itemIndex), oppure (-1, -1) se annullato
    std::pair<int, int> show();

private:
    WINDOW *parent;
    std::vector<std::string> menuTitles;
    std::vector<std::vector<std::string>> menuItems;

    void drawBar(int highlight);
    int showDropdown(int menuIndex, int startx);
    std::vector<int> menuStartX;
};

#endif
