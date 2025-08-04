#ifndef MENUBAR_H
#define MENUBAR_H

#include <ncurses.h>
#include <vector>
#include <string>

enum class MenuCommand {
    SaveExit,
    Quit,
    Save,
    EditConcertInfo,
	DeleteConcert,
    AddMusician,
    EditMusician,
    DeleteMusician,
    AddPiece,
    EditPiece,
    DeletePiece,
    AddRehearsal,
    EditRehearsal,
    DeleteRehearsal,
	AddScore,
	DeleteScore,
	ViewScore,
    Comment,
	Mail,
    Todo,
	Youtube,
	HTML,
    None
};

struct MenuItem {
    std::string label;
    MenuCommand command;
};

class MenuBar {
public:
	MenuBar();
    explicit MenuBar(WINDOW* win);
    MenuCommand show() const;

    void setTitles(const std::vector<std::string>& titles);
    void setItems(const std::vector<std::vector<MenuItem>>& items);

private:
    WINDOW* parent;
    std::vector<std::string> menuTitles;
    std::vector<std::vector<MenuItem>> menuItems;
    std::vector<int> menuStartX;

    void drawBar(int highlight) const;
    int showDropdown(int menuIndex, int startx) const;

};

#endif // MENUBAR_H
