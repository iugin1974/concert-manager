#include "concert_form.h"

#include <form.h>
#include <ncurses.h>
#include <locale.h>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

static std::string trim(const char* buffer) {
    std::string str(buffer);
    str.erase(str.find_last_not_of(" \n") + 1);
    str.erase(0, str.find_first_not_of(" "));
    return str;
}

static std::vector<std::string> split(const std::string& input, char sep) {
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string item;
    while (getline(ss, item, sep)) {
        item.erase(0, item.find_first_not_of(" "));
        item.erase(item.find_last_not_of(" ") + 1);
        if (!item.empty())
            result.push_back(item);
    }
    return result;
}

Concert inputConcert(const Concert* existing) {
    FIELD* fields[3];
    fields[0] = new_field(1, 50, 0, 18, 0, 0);  // Places
    fields[1] = new_field(1, 50, 1, 18, 0, 0);  // Dates
    fields[2] = nullptr;

    for (int i = 0; i < 2; ++i) {
        set_field_back(fields[i], A_UNDERLINE);
        field_opts_off(fields[i], O_AUTOSKIP);
    }

    if (existing) {
        std::string places = "";
        for (const auto& place : existing->getPlaces()) places += place + ", ";
        std::string dates = "";
        for (const auto& d : existing->getDatesAsString()) dates += d + ", ";
        set_field_buffer(fields[0], 0, places.c_str());
        set_field_buffer(fields[1], 0, dates.c_str());
    }

    FORM* form = new_form(fields);
    WINDOW* win = newwin(10, 80, 4, 4);
    box(win, 0, 0);
    keypad(win, TRUE);
    set_form_win(form, win);
    set_form_sub(form, derwin(win, 6, 76, 1, 1));

    mvwprintw(win, 1, 2, "Places (comma-separated):");
    mvwprintw(win, 2, 2, "Dates (YYYY-MM-DD, comma-separated):");

    post_form(form);
    wrefresh(win);
    form_driver(form, REQ_FIRST_FIELD);

    int ch;
    while ((ch = wgetch(win)) != '\n') {
        switch (ch) {
            case KEY_DOWN:
            case 9:
                form_driver(form, REQ_NEXT_FIELD);
                form_driver(form, REQ_END_LINE);
                break;
            case KEY_UP:
            case KEY_BTAB:
                form_driver(form, REQ_PREV_FIELD);
                form_driver(form, REQ_END_LINE);
                break;
            case KEY_BACKSPACE:
            case 127:
            case '\b':
                form_driver(form, REQ_DEL_PREV);
                break;
            case KEY_DC:
                form_driver(form, REQ_DEL_CHAR);
                break;
            default:
                form_driver(form, ch);
                break;
        }
    }

    form_driver(form, REQ_VALIDATION);
    std::string placesStr = trim(field_buffer(fields[0], 0));
    std::string datesStr  = trim(field_buffer(fields[1], 0));

    std::vector<std::string> places = split(placesStr, ',');
    std::vector<std::string> dates  = split(datesStr, ',');

    unpost_form(form);
    free_form(form);
    for (int i = 0; i < 2; ++i) free_field(fields[i]);
    delwin(win);

    Concert concert;
    concert.setPlaces(places);
    concert.setDatesAsString(dates);
    return concert;
}
