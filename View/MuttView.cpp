#include "MuttView.h"
#include <ncurses.h>

void MuttView::launchMutt(std::string &adress) {
	def_prog_mode();   // Salva lo stato corrente di ncurses
	endwin();          // Sospende ncurses

	// Lancia Mut con system()
	std::string command = "mutt " + adress;
	system(command.c_str());

	// Riattiva ncurses
	reset_prog_mode(); // Ripristina modalità ncurses
	refresh();         // Ridisegna
}
