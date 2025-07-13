#ifndef NCURSES_MENU_BAR_H
#define NCURSES_MENU_BAR_H

#include <string>
#include <vector>
#include <utility>

class NcursesMenuBar
{
public:
    NcursesMenuBar(const std::vector<std::string>& menuTitles,
                   const std::vector<std::vector<std::string>>& menuItems);
    ~NcursesMenuBar();

    std::pair<int, int> activate(); // returns <menu_index, item_index>

private:
    std::vector<std::string> menuTitles;
    std::vector<std::vector<std::string>> menuItemsList;

    void drawBar(int highlightIndex);
    std::pair<int, int> openMenu(int index);
};

#endif // NCURSES_MENU_BAR_H
