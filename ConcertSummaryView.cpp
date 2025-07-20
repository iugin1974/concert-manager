/// ConcertSummaryView.cpp
#include "ConcertSummaryView.h"
#include "logMessage.h"
#include "Utils.h"
#include <ncurses.h>
#include <fstream>
#include <sstream>

void ConcertSummaryView::setConcert(const Concert &c) {
	concert = c;
}

MenuCommand ConcertSummaryView::show() {
	const auto &pieces = concert.getProgram();
	const int pageSize = 10;

	while (true) {
		clear();
		int row = 1;

		attron(A_BOLD);
		mvprintw(row++, 2, "Concert Summary");
		mvprintw(row++, 2, "---------------");
		mvprintw(row++, 2, "%s", concert.getTitle().c_str());
		row++;
		attroff(A_BOLD);

		const auto &places = concert.getPlaces();
		const auto &dates = concert.getDatesAsString();

		LOG_MSG("places.size() = " + std::to_string(places.size()));
		LOG_MSG("dates.size() = " + std::to_string(dates.size()));

		for (size_t i = 0; i < places.size(); ++i) {
			mvprintw(row++, 2, "- %s - %s", dates[i].c_str(),
					places[i].c_str());
		}

		row += 2;
		attron(A_BOLD);
		mvprintw(row++, 2, "Rehearsals:");
		attroff(A_BOLD);
		mvprintw(row++, 2, "%-12s %-6s %-20s %-20s", "Date", "Time", "Place",
				"Musicians");
		mvhline(row++, 2, ACS_HLINE, 70);

		for (const auto &r : concert.getRehearsals()) {
			mvprintw(row++, 2, "%-12s %-6s %-20s %-20s",
					dateToString(r.getDate()).c_str(), r.getStartTime().c_str(),
					r.getPlace().c_str(), r.getMusicians().c_str());
		}

		mvhline(row++, 2, ACS_HLINE, 70);
		row += 2;

		attron(A_BOLD);
		mvprintw(row++, 2, "Program:");
		attroff(A_BOLD);
		mvprintw(row++, 2, "%-20s %-30s %-8s %s", "Composer", "Title",
				"Duration", "Instruments");
		mvhline(row++, 2, ACS_HLINE, 90);

		for (const auto &p : pieces) {
			std::stringstream org;
			org << p.getInstruments();
			if (!p.getSingerPart().empty())
				org << " - " << p.getSingerPart();
			if (p.hasChoir())
				org << " - Choir";

			mvprintw(row++, 2, "%-20s %-30s %-8s %s", p.getComposer().c_str(),
					p.getTitle().c_str(),
					convertToMMSS(p.getDuration()).c_str(), org.str().c_str());
		}

		mvhline(row++, 2, ACS_HLINE, 90);
		mvprintw(row++, 54, "%-8s",
				convertToMMSS(concert.getDuration()).c_str());

		row += 2;
		attron(A_BOLD);
		mvprintw(row++, 2, "Musicians:");
		attroff(A_BOLD);
		mvprintw(row++, 2, "%-20s %-20s %10s", "Name", "Instrument", "Gage");
		mvhline(row++, 2, ACS_HLINE, 55);

		double totalGage = 0.0;
		for (const auto &m : concert.getMusicians()) {
			mvprintw(row++, 2, "%-20s %-20s %10.2f", m.getName().c_str(),
					m.getInstrument().c_str(), m.getGage());
			totalGage += m.getGage();
		}

		mvhline(row++, 2, ACS_HLINE, 55);
		mvprintw(row++, 38, "Total gage: %.2f CHF", totalGage);

		if (!concert.getComment().empty()) {
			row += 2;
			attron(A_BOLD);
			mvprintw(row++, 2, "Comment:");
			attroff(A_BOLD);
			mvprintw(row++, 0, "%s", concert.getComment().c_str());
		}

		refresh();

		int ch = getch();
		if (ch == KEY_F(2)) {
			MenuCommand result = menuBar.show();
			if (result != MenuCommand::None)
				return result;
		}
	}
}

void ConcertSummaryView::closeForm() {
	clear();
	refresh();
}

void ConcertSummaryView::validateFields() {

}
