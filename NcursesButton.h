#ifndef NCURSES_BUTTON_H
#define NCURSES_BUTTON_H

#include <form.h>
#include <string>

class NcursesButton {
private:
    FIELD* field;
    std::string label;
    int row, col, width;

public:
    NcursesButton(const std::string& text, int y, int x, int w = 12);
    ~NcursesButton();

    FIELD* getField() const;
    void setActive(bool active);
    void refresh();
};

#endif

