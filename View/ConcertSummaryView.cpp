/// ConcertSummaryView.cpp
#include "ConcertSummaryView.h"
#include "logMessage.h"
#include "Utils.h"
#include <ncurses.h>
#include <fstream>
#include <sstream>

ConcertSummaryView::~ConcertSummaryView() {
	closeForm();
}

void ConcertSummaryView::setConcert(const Concert &c) {
	concert = c;
}

void ConcertSummaryView::init_form() {}

void ConcertSummaryView::show() {
	const auto &pieces = concert.getProgram();

		int row = 1;

		attron(A_BOLD);
		mvprintw(row++, 2, "Concert Summary");
		mvhline(row++, 2, ACS_HLINE, 15);
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
		// stampa intestazione
		mvprintw(row++, 2, "%s", Rehearsal::header().c_str());
		mvhline(row++, 2, ACS_HLINE, 70);

		// stampa righe
		for (const auto &r : concert.getRehearsals()) {
		    mvprintw(row++, 2, "%s", r.toString().c_str());
		}

		mvhline(row++, 2, ACS_HLINE, 70);
		row += 2;

		attron(A_BOLD);
		mvprintw(row++, 2, "Program:");
		attroff(A_BOLD);
		mvprintw(row++, 2, "%s", MusicalPiece::header().c_str());
		mvhline(row++, 2, ACS_HLINE, 130);

		for (const auto &p : pieces) {
		    mvprintw(row++, 2, "%s", p.toString().c_str());
		}

		mvhline(row++, 2, ACS_HLINE, 130);
		mvprintw(row++, 54, "%-8s",
				convertToMMSS(concert.getDuration()).c_str());

		row += 2;
		attron(A_BOLD);
		mvprintw(row++, 2, "Musicians:");
		attroff(A_BOLD);
		mvprintw(row++, 2, "%s", Musician::header().c_str());
		mvhline(row++, 2, ACS_HLINE, 90);

		double totalGage = 0.0;
		double totalGageWithTravelCosts = 0.0;

		for (const auto &m : concert.getMusicians()) {
		    mvprintw(row++, 2, "%s", m.toString().c_str());
		    totalGage += m.getGage();
		    totalGageWithTravelCosts += m.getFullPayment();
		}

		mvhline(row++, 2, ACS_HLINE, 90);
		mvprintw(row++, 2, "%-41s %10.2f %20.2f", "Total:", totalGage, totalGageWithTravelCosts);

		if (!concert.getComment().empty()) {
			row += 2;
			attron(A_BOLD);
			mvprintw(row++, 2, "Comment:");
			attroff(A_BOLD);
			mvprintw(row++, 0, "%s", concert.getComment().c_str());
		}

		refresh();
	}

	MenuCommand ConcertSummaryView::getCommand() {
		while (true) {
		int ch = getch();
		if (ch == KEY_F(2)) {
			MenuCommand result = menuBar.show();
			if (result != MenuCommand::None)
				return result;
		} else if (ch == KEY_UP) {

		} else if (ch == KEY_DOWN) {

		}
	}
}

void ConcertSummaryView::closeForm() {
}

void ConcertSummaryView::saveDataFromForm() {}
