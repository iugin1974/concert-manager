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
		mvprintw(row++, 2, "%-20s %-40s %-8s %-35s %-3s", "Composer", "Title",
				"Duration", "Instruments", "Scores");
		mvhline(row++, 2, ACS_HLINE, 130);

		for (const auto &p : pieces) {
			std::stringstream org;
			org << p.getInstruments();
			if (!p.getSingerPart().empty())
				org << " - " << p.getSingerPart();
			if (p.hasChoir())
				org << " - Choir";

			mvprintw(row++, 2, "%-20s %-40s %-8s %-35s %-3zu", p.getComposer().c_str(),
					p.getTitle().c_str(),
					convertToMMSS(p.getDuration()).c_str(), org.str().c_str(), p.getScores().size());
		}

		mvhline(row++, 2, ACS_HLINE, 130);
		mvprintw(row++, 54, "%-8s",
				convertToMMSS(concert.getDuration()).c_str());

		row += 2;
		attron(A_BOLD);
		mvprintw(row++, 2, "Musicians:");
		attroff(A_BOLD);
		mvprintw(row++, 2, "%-20s %-20s %10s %20s", "Name", "Instrument", "Gage", "Total with expenses");
		mvhline(row++, 2, ACS_HLINE, 90);

		double totalGage = 0.0;
		double totalGageWithTravelCosts = 0.0;

		for (const auto &m : concert.getMusicians()) {
			double travelCosts = m.getTravelCosts() * (
				(m.getRehearsalNumber() * 2) +
				(m.getConcertNumber() * 2));

			double fullPayment = m.getGage() + travelCosts;

			mvprintw(row++, 2, "%-20s %-20s %10.2f %20.2f",
				m.getName().c_str(),
				m.getInstrument().c_str(),
				m.getGage(),
				fullPayment);

			totalGage += m.getGage();
			totalGageWithTravelCosts += fullPayment;
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
