#include "ConcertController.h"
#include "MainMenuView.h"
#include <vector>
#include <ncurses.h>
#include <algorithm> // std::sort

int main()
{
	initscr();
	cbreak();
	noecho();
	initscr(); // inizializza ncurses
	setlocale(LC_ALL, "");
	// start_color();        // abilita i colori

	// Definisci la coppia 1: testo bianco (COLOR_WHITE), sfondo blu (COLOR_BLUE)
	// init_pair(1, COLOR_WHITE, COLOR_BLUE);

	// Usa la coppia di colori 1 per tutto lo schermo
	// attron(COLOR_PAIR(1));
	// bkgd(COLOR_PAIR(1));  // imposta il background dello schermo
	keypad(stdscr, TRUE);

	ConcertController controller;
	std::vector<Concert> concerts;
	controller.loadXML(concerts);

	while (true)
	{
		MainMenuView menu;
		MainMenuView::Action action = menu.show();

		switch (action)
		{
		case MainMenuView::ADD_CONCERT:
		{
			auto concert = controller.createConcert();
			if (concert)
				concerts.push_back(*concert);
			controller.saveXML(concerts);
		}
		break;
		case MainMenuView::MANAGE_CONCERTS:
			controller.listConcerts(concerts);
			break;

		case MainMenuView::SAVE:
			controller.saveXML(concerts);
			break;

		case MainMenuView::EXIT:
			controller.saveXML(concerts); // salva prima di uscire
			endwin();
			return 0;
		}
	}
}
