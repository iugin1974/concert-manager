// ConcertSummaryView.cpp
#include "ConcertSummaryView.h"
#include "PopupMenu.h"
#include "Utils.h"
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
	  convertToMMSS(p.getDuration()).c_str(),
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
  const auto& pieces = concert.getProgram();
  const int pageSize = 10;
  int currentPage = 0;
  //int totalPages = (pieces.size() + pageSize - 1) / pageSize;

  while (true) {
    clear();
    int row = 1;

    attron(A_BOLD);
    mvprintw(row++, 2, "Concert Summary");
    mvprintw(row++, 2, "---------------");
    mvprintw(row++, 2, "%s", concert.getTitle().c_str());
    row ++;
    attroff(A_BOLD);
    const auto& places = concert.getPlaces();
    const auto& dates = concert.getDatesAsString(); 

    for (size_t i = 0; i < places.size(); ++i) {
      mvprintw(row++, 2, "- %s - %s", dates[i].c_str(), places[i].c_str());
    }

    row += 2;

	attron(A_BOLD);
    mvprintw(row++, 2, "Rehearsals:");
    attroff(A_BOLD);
    mvprintw(row++, 2, "%-12s %-6s %-20s %-20s", "Date", "Time", "Place", "Musicians");
    mvhline(row++, 2, ACS_HLINE, 70);

    for (const auto& r : concert.getRehearsals()) {
        mvprintw(row++, 2, "%-12s %-6s %-20s %-20s",
            dateToString(r.getDate()).c_str(),
            r.getStartTime().c_str(),
            r.getPlace().c_str(),
            r.getMusicians().c_str());
    }
mvhline(row++, 2, ACS_HLINE, 70);

row += 2;

    attron(A_BOLD);
    mvprintw(row++, 2, "Program:");
    attroff(A_BOLD);
    mvprintw(row++, 2, "%-20s %-25s %-8s %s", "Composer", "Title", "Duration", "Instruments");
    mvhline(row++, 2, ACS_HLINE, 90);

    int start = currentPage * pageSize;
    int end = std::min(start + pageSize, (int)pieces.size());
    for (int i = start; i < end; ++i) {
      const auto& p = pieces[i];
      std::stringstream org;
      org << p.getInstruments();
      if (!p.getSingerPart().empty()) org << " - " << p.getSingerPart();
      if (p.hasChoir()) org << " - Choir";

      mvprintw(row++, 2, "%-20s %-25s %-8s %s",
	  p.getComposer().c_str(),
	  p.getTitle().c_str(),
	  convertToMMSS(p.getDuration()).c_str(),
	  org.str().c_str());
    }

    mvhline(row++, 2, ACS_HLINE, 90);
    mvprintw(row++, 49, "%-8s", convertToMMSS(concert.getDuration()).c_str());
//
//
//
    row+=2;
    attron(A_BOLD);
    mvprintw(row++, 2, "Musicians:");
    attroff(A_BOLD);
    mvprintw(row++, 2, "%-20s %-20s %10s", "Name", "Instrument", "Gage");
    mvhline(row++, 2, ACS_HLINE, 55);

    double totalGage = 0.0;
    for (const auto& m : concert.getMusicians()) {
      mvprintw(row++, 2, "%-20s %-20s %10.2f",
	  m.getName().c_str(),
	  m.getInstrument().c_str(),
	  m.getGage());
      totalGage += m.getGage();
    }

    mvhline(row++, 2, ACS_HLINE, 55);
    //
    //
    //

    mvprintw(row++, 38, "Total gage: %.2f CHF", totalGage);

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
	"Print to file"
      };

      PopupMenu menu(stdscr, actions);
      int selected = menu.show();

      switch (selected) {
	case 0: // Exit
	  return;
	  /*
	     case 1: { 
	     viewProgram(concert);
	     break;
	     }
	     */
	case 1: { // Print to file
		  std::ofstream out("concert_summary.txt");
		  if (!out) break;

		  out << "Concert: " << concert.getTitle() << "\n\n";
		  out << "Places:\n";
		  for (const auto& p : concert.getPlaces()) out << "  - " << p << "\n";
		  out << "Dates:\n";
		  for (const auto& d : concert.getDatesAsString()) out << "  - " << d << "\n";

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
