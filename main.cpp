#include "ConcertController.h"

#include <vector>
#include <ncurses.h>


int main()
{
	initscr();
	cbreak();
	noecho();
	initscr(); // inizializza ncurses
	setlocale(LC_ALL, "");
	start_color();        // abilita i colori
use_default_colors();
init_pair(1, COLOR_WHITE, COLOR_BLUE);   // barra principale
init_pair(2, COLOR_BLACK, COLOR_CYAN);   // menu selezionato
init_pair(3, COLOR_YELLOW, -1);          // evidenziato

	// Definisci la coppia 1: testo bianco (COLOR_WHITE), sfondo blu (COLOR_BLUE)
	// init_pair(1, COLOR_WHITE, COLOR_BLUE);

	// Usa la coppia di colori 1 per tutto lo schermo
	// attron(COLOR_PAIR(1));
	// bkgd(COLOR_PAIR(1));  // imposta il background dello schermo
	keypad(stdscr, TRUE);

	ConcertController controller;
	controller.start();
	endwin();
return 0;

}
