#include "NcursesMenuBar.h"
#include <menu.h>
#include <ncurses.h>
#include <string>
#include <vector>
#include <utility>

NcursesMenuBar::NcursesMenuBar(const std::vector<std::string>& titles, const std::vector<std::vector<std::string>>& items)
    : menuTitles(titles), menuItemsList(items)
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
}

NcursesMenuBar::~NcursesMenuBar()
{
    endwin();
}

void NcursesMenuBar::drawBar(int highlightIndex)
{
    attron(A_REVERSE);
    mvhline(0, 0, ' ', COLS);
    attroff(A_REVERSE);

    int x = 2;
    for (size_t i = 0; i < menuTitles.size(); ++i)
    {
        if ((int)i == highlightIndex)
            attron(A_REVERSE);
        else
            attroff(A_REVERSE);

        mvprintw(0, x, " %s ", menuTitles[i].c_str());
        x += menuTitles[i].length() + 4;
    }
    attroff(A_REVERSE);
    move(1, 0);
    refresh();
}

std::pair<int, int> NcursesMenuBar::activate()
{
    int index = 0;
    drawBar(index);
    return openMenu(index);
}

std::pair<int, int> NcursesMenuBar::openMenu(int index)
{
    const std::vector<std::string>& items = menuItemsList[index];
    int n_items = items.size();

    ITEM** menu_items = new ITEM*[n_items + 1];
    for (int i = 0; i < n_items; ++i)
    {
        menu_items[i] = new_item(items[i].c_str(), "");
    }
    menu_items[n_items] = nullptr;

    MENU* menu = new_menu(menu_items);
    WINDOW* menu_win = newwin(n_items + 2, 30, 1, 2 + index * 10);
    keypad(menu_win, TRUE);

    set_menu_win(menu, menu_win);
    set_menu_sub(menu, derwin(menu_win, n_items, 28, 1, 1));
    box(menu_win, 0, 0);
    post_menu(menu);
    wrefresh(menu_win);

    int c;
    int choice = -1;
    int currentIndex = index;

    while (true)
{
    c = wgetch(menu_win);
    ITEM* cur = current_item(menu);
    int pos = item_index(cur);

    switch (c)
    {
        case 10: // ENTER
            choice = pos;

            unpost_menu(menu);
            free_menu(menu);
            for (int i = 0; i < n_items; ++i)
                free_item(menu_items[i]);
            delete[] menu_items;
            delwin(menu_win);

            return std::make_pair(index, choice);

        case 27: // ESC
        case 'q':
            unpost_menu(menu);
            free_menu(menu);
            for (int i = 0; i < n_items; ++i)
                free_item(menu_items[i]);
            delete[] menu_items;
            delwin(menu_win);

            return std::make_pair(index, -1);

        case KEY_DOWN:
        case '\t':
            if (pos == n_items - 1)
                set_current_item(menu, menu_items[0]);
            else
                menu_driver(menu, REQ_DOWN_ITEM);
            break;

        case KEY_UP:
            if (pos == 0)
                set_current_item(menu, menu_items[n_items - 1]);
            else
                menu_driver(menu, REQ_UP_ITEM);
            break;

        case KEY_RIGHT:
        case KEY_LEFT:
            // Rimuovi il menu corrente
            unpost_menu(menu);
            free_menu(menu);
            for (int i = 0; i < n_items; ++i)
                free_item(menu_items[i]);
            delete[] menu_items;
            delwin(menu_win);

            // Pulizia dello schermo nella zona menu
            for (int row = 1; row <= n_items + 2; ++row)
                mvhline(row, 0, ' ', COLS);
            refresh();

            if (c == KEY_RIGHT)
                currentIndex = (currentIndex + 1) % menuTitles.size();
            else
                currentIndex = (currentIndex - 1 + menuTitles.size()) % menuTitles.size();

            drawBar(currentIndex);
            return openMenu(currentIndex);  // Ricorsione
        
        default:
            beep();
            break;
    }

    wrefresh(menu_win);
}

}
