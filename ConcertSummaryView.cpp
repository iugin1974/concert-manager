// ConcertSummaryView.cpp
#include "ConcertSummaryView.h"
#include <ncurses.h>

#include <fstream>
#include <sstream>
#include <iomanip> // per std::setw e std::setprecision
#include "ConcertSummaryView.h"
#include <ncurses.h>

void ConcertSummaryView::saveToFile(const Concert& concert) {
    std::ofstream out("concert_summary.txt");
    if (!out) return;

    out << "CONCERT SUMMARY\n\n";
    out << "Title: " << concert.getTitle() << "\n\n";

    out << "Places:\n";
    for (const auto& place : concert.getPlaces())
        out << "  - " << place << "\n";

    out << "\nDates:\n";
    for (const auto& date : concert.getDates())
        out << "  - " << date << "\n";

    out << "\nMusicians:\n";
    out << std::left << std::setw(20) << "Name"
        << std::setw(20) << "Instrument"
        << std::right << std::setw(10) << "Gage\n";
    out << std::string(50, '-') << "\n";

    double totalGage = 0.0;
    for (const auto& m : concert.getMusicians()) {
        out << std::left << std::setw(20) << m.getName()
            << std::setw(20) << m.getInstrument()
            << std::right << std::setw(10) << std::fixed << std::setprecision(2) << m.getGage() << "\n";
        totalGage += m.getGage();
    }

    out << "\nTotal gage: " << totalGage << " CHF\n";

    out << "\nProgram:\n";
    for (const auto& p : concert.getProgram()) {
        std::stringstream org;
        org << p.getInstruments();
        if (!p.getSingerPart().empty())
            org << " - " << p.getSingerPart();
        if (p.hasChoir())
            org << " - Choir";

        out << std::left << std::setw(20) << p.getComposer()
            << std::setw(25) << p.getTitle()
            << std::setw(10) << p.getDuration()
            << org.str() << "\n";
    }

    out.close();
}


void ConcertSummaryView::show(const Concert& concert) {
    clear();
    int row = 1;

    mvprintw(row++, 2, "Concert Summary");
    row++;
attron(A_BOLD);
    mvprintw(row++, 2, "%s", concert.getTitle().c_str());
attroff(A_BOLD);
    row++;
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

    //
    // PROGRAMMA
    //
    row += 2;
    mvprintw(row++, 2, "Program:");
    mvprintw(row++, 4, "%-20s %-25s %-8s %s", "Composer", "Title", "Duration", "Ensemble");
    mvhline(row++, 4, ACS_HLINE, 80);

    for (const auto& piece : concert.getProgram()) {
        std::string instruments = piece.getInstruments();
        std::string singerPart = piece.getSingerPart();
        std::string ensemble;

        if (!instruments.empty())
            ensemble += instruments;
        if (!singerPart.empty()) {
            if (!ensemble.empty()) ensemble += " - ";
            ensemble += singerPart;
        }
        if (piece.hasChoir()) {
            if (!ensemble.empty()) ensemble += " - ";
            ensemble += "Choir";
        }

        mvprintw(row++, 4, "%-20s %-25s %-8s %s",
                 piece.getComposer().c_str(),
                 piece.getTitle().c_str(),
                 piece.getDuration().c_str(),
                 ensemble.c_str());
    }

    row += 2;
   mvprintw(row++, 2, "Press any key to return... (F3 to save to file)");
refresh();

int ch = getch();
if (ch == KEY_F(3)) {
    saveToFile(concert);
    mvprintw(row++, 2, "Saved to concert_summary.txt");
    refresh();
    getch(); // aspetta un secondo invio
}
}
