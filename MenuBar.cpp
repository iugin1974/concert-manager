#include "MenuBar.h"
#define KEY_ESC 27

MenuBar::MenuBar() {
	parent = nullptr;
}
MenuBar::MenuBar(WINDOW *win) :
		parent(win) {
}

void MenuBar::setTitles(const std::vector<std::string> &titles) {
	menuTitles = titles;
	menuStartX.clear();

	int x = 1;
	for (const auto &title : titles) {
		menuStartX.push_back(x);
		x += static_cast<int>(title.size()) + 4; // padding
	}
}

void MenuBar::setItems(const std::vector<std::vector<MenuItem>> &items) {
	menuItems = items;
}

void MenuBar::drawBar(int highlight) const {
	wattron(parent, A_REVERSE);
	for (size_t i = 0; i < menuTitles.size(); ++i) {
		if (static_cast<int>(i) == highlight)
			wattron(parent, A_BOLD);

		mvwprintw(parent, 0, menuStartX[i], " %s ", menuTitles[i].c_str());

		if (static_cast<int>(i) == highlight)
			wattroff(parent, A_BOLD);
	}
	wattroff(parent, A_REVERSE);
	wrefresh(parent);
}

int MenuBar::showDropdown(int menuIndex, int startx) const {
	if (menuIndex >= static_cast<int>(menuItems.size()))
		return -1;

	const auto &items = menuItems[menuIndex];
	int height = static_cast<int>(items.size()) + 2;
	int width = 0;

	for (const auto &item : items)
		width = std::max(width, static_cast<int>(item.label.size()) + 2);

	WINDOW *menuWin = newwin(height, width, 1, startx);
	box(menuWin, 0, 0);
	keypad(menuWin, TRUE);

	int highlight = 0;
	while (true) {
		for (size_t i = 0; i < items.size(); ++i) {
			if (static_cast<int>(i) == highlight)
				wattron(menuWin, A_REVERSE);
			mvwprintw(menuWin, static_cast<int>(i) + 1, 1, "%-*s", width - 2,
					items[i].label.c_str());
			if (static_cast<int>(i) == highlight)
				wattroff(menuWin, A_REVERSE);
		}
		wrefresh(menuWin);

		int c = wgetch(menuWin);
		switch (c) {
		case KEY_UP:
			highlight = (highlight - 1 + static_cast<int>(items.size()))
					% static_cast<int>(items.size());
			break;
		case KEY_DOWN:
			highlight = (highlight + 1) % static_cast<int>(items.size());
			break;
		case KEY_ENTER:
		case 10:
			delwin(menuWin);      // rimuovi menu
			touchwin(parent);     // forza il ridisegno della finestra sotto
			wrefresh(parent);     // aggiorna schermo
			return highlight;

		case KEY_ESC:
			delwin(menuWin);      // rimuovi menu
			touchwin(parent);     // forza il ridisegno della finestra sotto
			wrefresh(parent);     // aggiorna schermo
			return -1;

		case KEY_RIGHT:
			delwin(menuWin);      // rimuovi menu
			touchwin(parent);     // forza il ridisegno della finestra sotto
			wrefresh(parent);     // aggiorna schermo
			return -2;

		case KEY_LEFT:
			delwin(menuWin);      // rimuovi menu
			touchwin(parent);     // forza il ridisegno della finestra sotto
			wrefresh(parent);     // aggiorna schermo
			return -3;
		}
	}
}

MenuCommand MenuBar::show() const {
	int current = 0;
	drawBar(current);
	keypad(parent, TRUE);

	while (true) {
		int ch = wgetch(parent);
		switch (ch) {
		case KEY_LEFT:
			current = (current - 1 + static_cast<int>(menuTitles.size()))
					% static_cast<int>(menuTitles.size());
			drawBar(current);
			break;
		case KEY_RIGHT:
			current = (current + 1) % static_cast<int>(menuTitles.size());
			drawBar(current);
			break;
		case KEY_DOWN: {
			while (true) {
				int item = showDropdown(current, menuStartX[current]);

				if (item >= 0) {
					return menuItems[current][item].command;
				} else if (item == -2) { // freccia destra
					current = (current + 1)
							% static_cast<int>(menuTitles.size());
				} else if (item == -3) { // freccia sinistra
					current =
							(current - 1 + static_cast<int>(menuTitles.size()))
									% static_cast<int>(menuTitles.size());
				} else { // esc o errore
					drawBar(current);
					break;
				}
			}
			break;
		}

		case KEY_ESC:
			return MenuCommand::None;
		}
	}
}
