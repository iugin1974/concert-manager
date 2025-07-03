#ifndef POPUP_MENU_H
#define POPUP_MENU_H

#include <vector>
#include <string>
#include <ncurses.h>

class PopupMenu {
public:
    PopupMenu(WINDOW* parent, const std::vector<std::string>& options,
              int x = 10, int y = 10);
    int show();  // Ritorna indice selezionato o -1 se cancellato

private:
    WINDOW* parent;
    std::vector<std::string> options;
    int posX, posY;
};

#endif

