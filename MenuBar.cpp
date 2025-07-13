#include "MenuBar.h"

MenuBar::MenuBar(WINDOW *win,
                 const std::vector<std::string>& titles,
                 const std::vector<std::vector<std::string>>& items)
    : parent(win), menuTitles(titles), menuItems(items)
{
    // Calcola la posizione X di ogni menu nella barra
    int x = 1;
    for (const auto& title : titles) {
        menuStartX.push_back(x);
        x += title.size() + 4; // padding
    }
}

void MenuBar::drawBar(int highlight)
{
    werase(parent);
    wattron(parent, A_REVERSE);
    for (size_t i = 0; i < menuTitles.size(); ++i) {
        if ((int)i == highlight)
            wattron(parent, A_BOLD);

        mvwprintw(parent, 0, menuStartX[i], " %s ", menuTitles[i].c_str());

        if ((int)i == highlight)
            wattroff(parent, A_BOLD);
    }
    wattroff(parent, A_REVERSE);
    wrefresh(parent);
}

int MenuBar::showDropdown(int menuIndex, int startx)
{
    const auto& items = menuItems[menuIndex];
    int height = items.size() + 2;
    int width = 0;
    for (const auto& item : items)
        if ((int)item.size() + 2 > width)
            width = item.size() + 2;

    WINDOW *menuWin = newwin(height, width, 1, startx);
    box(menuWin, 0, 0);
    keypad(menuWin, TRUE);

    int highlight = 0;
    while (1)
    {
        for (size_t i = 0; i < items.size(); ++i)
        {
            if ((int)i == highlight)
                wattron(menuWin, A_REVERSE);
            mvwprintw(menuWin, i + 1, 1, "%-*s", width - 2, items[i].c_str());
            if ((int)i == highlight)
                wattroff(menuWin, A_REVERSE);
        }
        wrefresh(menuWin);

        int c = wgetch(menuWin);
        switch (c)
        {
        case KEY_UP:
            highlight = (highlight - 1 + items.size()) % items.size();
            break;
        case KEY_DOWN:
            highlight = (highlight + 1) % items.size();
            break;
        case 10: // Invio
            delwin(menuWin);
            return highlight;
        case 27: // ESC
            delwin(menuWin);
            return -1;
        }
    }
}

std::pair<int, int> MenuBar::show()
{
    int current = 0;
    drawBar(current);
    keypad(parent, TRUE);

    while (1)
    {
        int ch = wgetch(parent);
        switch (ch)
        {
        case KEY_LEFT:
            current = (current - 1 + menuTitles.size()) % menuTitles.size();
            drawBar(current);
            break;
        case KEY_RIGHT:
            current = (current + 1) % menuTitles.size();
            drawBar(current);
            break;
        case 10: // ENTER
        {
            int item = showDropdown(current, menuStartX[current]);
            if (item != -1)
                return {current, item};
            else
                drawBar(current); // Redraw menu bar after cancel
            break;
        }
        case 27: // ESC
            return {-1, -1};
        }
    }
}
