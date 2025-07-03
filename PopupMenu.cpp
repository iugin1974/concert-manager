#include "PopupMenu.h"

PopupMenu::PopupMenu(WINDOW* parent, const std::vector<std::string>& options,
                     int x, int y)
    : parent(parent), options(options), posX(x), posY(y) {}

int PopupMenu::show() {
    int n_choices = options.size();
    int highlight = 0;
    int choice = -1;
    int ch;

    int width = 0;
    for (const auto& opt : options)
        if (opt.size() > width) width = opt.size();
    width += 4;

    int height = n_choices + 2;
    WINDOW* menu_win = newwin(height, width, posY, posX);
    box(menu_win, 0, 0);
    keypad(menu_win, TRUE);
    wrefresh(menu_win);

    while (true) {
        for (int i = 0; i < n_choices; ++i) {
            if (i == highlight)
                wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, i + 1, 2, options[i].c_str());
            wattroff(menu_win, A_REVERSE);
        }
        ch = wgetch(menu_win);
        switch (ch) {
            case KEY_UP:
                highlight = (highlight - 1 + n_choices) % n_choices;
                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % n_choices;
                break;
            case 10: // ENTER
                choice = highlight;
                goto end;
            case 27: // ESC
                goto end;
        }
    }

end:
    werase(menu_win);
    wrefresh(menu_win);
    delwin(menu_win);
    touchwin(parent);
    wrefresh(parent);
    return choice;
}

