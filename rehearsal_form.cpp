#include "rehearsal_form.h"
#include "PopupMenu.h"
#include "NcursesMenuBar.h"
#include <form.h>
#include <ncurses.h>
#include <string>
#include <vector>
#include "Utils.h" // trim, dateToString, stringToDate
#include <ctime>

bool runRehearsalForm(const Rehearsal *existing, Rehearsal &rehearsal)
{
    FIELD *fields[5];                          // 4 fields + null
    fields[0] = new_field(1, 20, 2, 20, 0, 0); // Date
    fields[1] = new_field(1, 10, 4, 20, 0, 0); // Start time
    fields[2] = new_field(1, 40, 6, 20, 0, 0); // Place
    fields[3] = new_field(1, 40, 8, 20, 0, 0); // Musicians
    fields[4] = nullptr;

    for (int i = 0; i < 4; ++i)
    {
        set_field_back(fields[i], A_UNDERLINE);
        field_opts_off(fields[i], O_AUTOSKIP);
    }

    FORM *form = new_form(fields);
    post_form(form);
    refresh();

    mvprintw(2, 2, "Date (DD.MM.YYYY):");
    mvprintw(4, 2, "Start time (HH:MM):");
    mvprintw(6, 2, "Place:");
    mvprintw(8, 2, "Musicians:");

    if (existing)
    {
        set_field_buffer(fields[0], 0, dateToString(existing->getDate()).c_str());
        set_field_buffer(fields[1], 0, existing->getStartTime().c_str());
        set_field_buffer(fields[2], 0, existing->getPlace().c_str());
        set_field_buffer(fields[3], 0, existing->getMusicians().c_str());
    }

    int ch;
    bool done = false;
    while (!done)
    {
        ch = getch();
        switch (ch)
        {
        case 10:
            form_driver(form, REQ_NEXT_FIELD);
            form_driver(form, REQ_END_LINE);
            break;
        case KEY_DOWN:
        case 9:
            form_driver(form, REQ_NEXT_FIELD);
            break;
        case KEY_UP:
            form_driver(form, REQ_PREV_FIELD);
            break;
        case KEY_BACKSPACE:
        case 127:
            form_driver(form, REQ_DEL_PREV);
            break;
        case KEY_F(2):
{
    std::vector<std::string> menuTitles = {"File"};
    std::vector<std::vector<std::string>> menuItems = {
        {"Save & Exit", "Exit without saving"}
    };

    NcursesMenuBar bar(menuTitles, menuItems);
    std::pair<int, int> result = bar.activate();

    if (result.first == 0) {
        switch (result.second)
        {
        case 0: // ✅ Save and Exit
        {
            form_driver(form, REQ_VALIDATION);
            std::string dateStr = trim(field_buffer(fields[0], 0));
            std::string start = trim(field_buffer(fields[1], 0));
            std::string place = trim(field_buffer(fields[2], 0));
            std::string musicians = trim(field_buffer(fields[3], 0));

            std::tm date;
            if (!stringToDate(dateStr, date)) {
                mvprintw(12, 2, "Invalid date format. Press any key.");
                getch();
                break;
            }

            rehearsal.setDate(date);
            rehearsal.setStartTime(start);
            rehearsal.setPlace(place);
            rehearsal.setMusicians(musicians);
            unpost_form(form);
            free_form(form);
            for (int i = 0; i < 4; ++i)
                free_field(fields[i]);

            done = true;
            return true;
        }
        case 1: // ❌ Exit without saving
            unpost_form(form);
            free_form(form);
            for (int i = 0; i < 4; ++i)
                free_field(fields[i]);
            return false;
        }
    }
    break;
}
default:
                form_driver(form, ch);
                break;
            }
    }
    return true;
}
