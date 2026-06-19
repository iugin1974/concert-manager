#pragma once
#include <vector>
#include <ncurses.h>
#include "Concert.h"

class MoveElementView {
public:
	template <typename T>
	void show(std::vector<T>& elements) {
		// ciclo principale
		int selectedIndex = 0;
		bool editing = false;
		
		initscr();
		keypad(stdscr, TRUE);
		noecho();
		
		while (true) {
			clear();
			
			// stampa lista
			for (size_t i = 0; i < elements.size(); ++i) {
				if ((int) i == selectedIndex)
					attron(A_REVERSE);
				mvprintw(i, 0, "%s", elements[i].toString().c_str());
				if ((int) i == selectedIndex)
					attroff(A_REVERSE);
			}
			
			// istruzioni a video
			mvprintw(elements.size() + 1, 0,
					 editing ?
					 "Editing mode: use arrows to move, ENTER to confirm" :
					 "Use arrows to select, ENTER to edit, q to quit");
			
			refresh();
			
			int ch = getch();
			
			if (ch == 'q' && !editing) {
				break; // esci
			}
			
			if (!editing) {
				switch (ch) {
					case KEY_UP:
						if (selectedIndex > 0)
							selectedIndex--;
					break;
					case KEY_DOWN:
						if (selectedIndex < (int) elements.size() - 1)
							selectedIndex++;
					break;
					case 10: // ENTER
						editing = true;
						break;
				}
			} else {
				switch (ch) {
					case KEY_UP: {
						if (selectedIndex > 0) {
							std::swap(elements.at(selectedIndex), elements.at(selectedIndex-1));
							selectedIndex--;
						}
					}
					break;
					case KEY_DOWN: {
						if (selectedIndex < static_cast<int>(elements.size()) - 1) {
							std::swap(elements.at(selectedIndex), elements.at(selectedIndex+1));
							selectedIndex++;
						}
					}
					break;
					case 10: // ENTER
						editing = false;
						break;
				}
			}
		}
		
		endwin();
	}
private:
};
