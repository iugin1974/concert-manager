#include "NcursesButton.h"
#include <ncurses.h>

NcursesButton::NcursesButton(const std::string& text, int y, int x, int w)
    : label(text), row(y), col(x), width(w) {
    
    field = new_field(1, width, row, col, 0, 0);
    set_field_buffer(field, 0, label.c_str());
    field_opts_off(field, O_EDIT);
    set_field_back(field, COLOR_PAIR(2));
    set_field_fore(field, COLOR_PAIR(2));
}

NcursesButton::~NcursesButton() {
    free_field(field);
}

FIELD* NcursesButton::getField() const {
    return field;
}

void NcursesButton::setActive(bool active) {
    if (active) {
        set_field_back(field, COLOR_PAIR(1));
        set_field_fore(field, COLOR_PAIR(1));
    } else {
        set_field_back(field, COLOR_PAIR(2));
        set_field_fore(field, COLOR_PAIR(2));
    }
}

void NcursesButton::refresh() {
    ::refresh();
}

