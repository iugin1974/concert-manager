// ConcertSummaryView.cpp
#include "ConcertSummaryView.h"
#include <ncurses.h>

void ConcertSummaryView::show(const Concert& concert) {
    clear();
    int row = 1;

    mvprintw(row++, 2, "Concert Summary");
    row++;
    mvprintw(row++, 2, "Title: %s", concert.getTitle().c_str());

    mvprintw(row++, 2, "Places:");
    for (const auto& place : concert.getPlaces())
        mvprintw(row++, 6, "- %s", place.c_str());

    mvprintw(row++, 2, "Dates:");
    for (const auto& date : concert.getDates())
        mvprintw(row++, 6, "- %s", date.c_str());

    row++;
    mvprintw(row++, 2, "Musicians:");
    mvprintw(row++, 4, "%-20s %-20s %10s", "Name", "Instrument", "Gage");
    mvhline(row++, 4, ACS_HLINE, 55);

    double totalGage = 0.0;
    for (const auto& m : concert.getMusicians()) {
        mvprintw(row++, 4, "%-20s %-20s %10.2f",
                 m.getName().c_str(),
                 m.getInstrument().c_str(),
                 m.getGage());
        totalGage += m.getGage();
    }

    row += 2;
    mvprintw(row++, 4, "Total gage: %.2f CHF", totalGage);
    row += 2;
    mvprintw(row++, 2, "Press any key to return...");
    refresh();
    getch();
}

