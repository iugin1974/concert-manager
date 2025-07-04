// ConcertSummaryView.cpp
#include "ConcertSummaryView.h"
#include "PopupMenu.h"
#include <ncurses.h>
#include <fstream>
#include <sstream>

void ConcertSummaryView::viewProgram(const Concert& concert) const {
		  const auto& pieces = concert.getProgram();
		  const int pageSize = 10;
		  int currentPage = 0;
		  int totalPages = (pieces.size() + pageSize - 1) / pageSize;

		  while (true) {
		    clear();
		    int r = 1;
		    attron(A_BOLD);
		    mvprintw(r++, 2, "Program (Page %d of %d)", currentPage + 1, totalPages);
		    attroff(A_BOLD);
		    r++;

		    mvprintw(r++, 2, "%-20s %-25s %-8s %s", "Composer", "Title", "Duration", "Instruments");
		    mvhline(r++, 2, ACS_HLINE, 90);

		    int start = currentPage * pageSize;
		    int end = std::min(start + pageSize, (int)pieces.size());
		    for (int i = start; i < end; ++i) {
		      const auto& p = pieces[i];
		      std::stringstream org;
		      org << p.getInstruments();
		      if (!p.getSingerPart().empty()) org << " - " << p.getSingerPart();
		      if (p.hasChoir()) org << " - Choir";

		      mvprintw(r++, 2, "%-20s %-25s %-8s %s",
			  p.getComposer().c_str(),
			  p.getTitle().c_str(),
			  p.getDuration().c_str(),
			  org.str().c_str());
		    }
			mvhline(r++, 2, ACS_HLINE, 90);
			mvprintw(r++, 2, "%s", concert.getComment().c_str());
		    r += 2;
		    mvprintw(r++, 2, "ESC: Return |  F3: Next  |  F4: Prev");
		    refresh();

		    int key = getch();
		    if (key == 27) break;
		    else if (key == KEY_F(3) && currentPage < totalPages - 1) currentPage++;
		    else if (key == KEY_F(4) && currentPage > 0) currentPage--;
		  }

}

void ConcertSummaryView::show(const Concert& concert) {
  while (true) {
    clear();
    int row = 1;

    attron(A_BOLD);
    mvprintw(row++, 2, "Concert Summary");
    attroff(A_BOLD);
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

    if (!concert.getComment().empty()) {
    row +=2;
    mvprintw(row++, 2, "Comment");
    mvprintw(row++, 2, "%s", concert.getComment().c_str());
    }

    row += 2;
    mvprintw(row++, 2, "Press F2 for options...");
    refresh();

    int ch = getch();
    if (ch == 27) {
	return;
    }
    else if (ch == 'v') {
viewProgram(concert);
    }
    else if (ch == KEY_F(2)) {
      std::vector<std::string> actions = {
	"Exit",
	"View Program",
	"Print to file"
      };

      PopupMenu menu(stdscr, actions);
      int selected = menu.show();

      switch (selected) {
	case 0: // Exit
	  return;

	case 1: { 
		  viewProgram(concert);
		  break;
		}

	case 2: { // Print to file
		  std::ofstream out("concert_summary.txt");
		  if (!out) break;

		  out << "Concert: " << concert.getTitle() << "\n\n";
		  out << "Places:\n";
		  for (const auto& p : concert.getPlaces()) out << "  - " << p << "\n";
		  out << "Dates:\n";
		  for (const auto& d : concert.getDates()) out << "  - " << d << "\n";

		  out << "\nMusicians:\n";
		  for (const auto& m : concert.getMusicians())
		    out << "  " << m.getName() << ", " << m.getInstrument()
		      << ", " << m.getGage() << " CHF\n";

		  out << "\nProgram:\n";
		  for (const auto& p : concert.getProgram()) {
		    std::stringstream org;
		    org << p.getInstruments();
		    if (!p.getSingerPart().empty()) org << " - " << p.getSingerPart();
		    if (p.hasChoir()) org << " - Choir";

		    out << "  " << p.getComposer() << ": " << p.getTitle()
		      << " (" << p.getDuration() << ") - " << org.str() << "\n";
		  }

		  out << "\nTotal gage: " << totalGage << " CHF\n";
		  out << "\nComment: " << concert.getComment() << "\n";
		  out << "_______________________________________________________________\n";                    
		  out.close();

		  mvprintw(row++, 2, "Saved to concert_summary.txt. Press any key...");
		  refresh();
		  getch();
		  break;
		}
      }
    }
  }
}
