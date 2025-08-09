#include "MovePieceView.h"
#include "Concert.h"
#include "ConcertController.h"
#include <ncurses.h>

void MovePieceView::show(Concert &concert, ConcertController &controller) {
	// ciclo principale
	int selectedIndex = 0;
	bool editing = false;

	initscr();
	keypad(stdscr, TRUE);
	noecho();

	while (true) {
		clear();

		// stampa lista
		const auto &program = concert.getProgram();
		for (size_t i = 0; i < program.size(); ++i) {
			if ((int) i == selectedIndex)
				attron(A_REVERSE);
			mvprintw(i, 0, "%s", program[i].toString().c_str());
			if ((int) i == selectedIndex)
				attroff(A_REVERSE);
		}

		// istruzioni a video
		mvprintw(program.size() + 1, 0,
				editing ?
						"Editing mode: use arrows to move, ENTER to confirm" :
						"Use arrows to select, ENTER to edit, q to quit");

		refresh();

		int ch = getch();

		if (ch == 'q' && !editing)
			break; // esci

		if (!editing) {
			switch (ch) {
			case KEY_UP:
				if (selectedIndex > 0)
					selectedIndex--;
				break;
			case KEY_DOWN:
				if (selectedIndex < (int) program.size() - 1)
					selectedIndex++;
				break;
			case 10: // ENTER
				editing = true;
				break;
			}
		} else {
			switch (ch) {
			case KEY_UP:
				if (controller.movePiece(concert, selectedIndex, -1))
					selectedIndex--;
				break;
			case KEY_DOWN:
				if (controller.movePiece(concert, selectedIndex, 1))
					selectedIndex++;
				break;
			case 10: // ENTER
				editing = false;
				break;
			}
		}
	}

	endwin();
}
