// ListSelectView.cpp
#include "ListSelectView.h"

#include "Concert.h"
#include "logMessage.h"
#include <ncurses.h>
#include <filesystem>
#include <algorithm>
#include <optional>

namespace fs = std::filesystem;

std::optional<std::vector<std::string>> ListSelectView::show(
		std::vector<std::string> availableItems) {
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);

	getmaxyx(stdscr, windowHeight, std::ignore);

	std::vector<std::string> selectedItems;

	draw(availableItems, selectedItems);

	while (true) {
		int ch = getch();
		if (std::isalpha(ch) && ch != 'A') {
		        char target = std::tolower(ch);
		        for (size_t i = 0; i < availableItems.size(); ++i) {
		            std::string filename =
		                std::filesystem::path(availableItems[i]).filename().string();
		            if (!filename.empty() && std::tolower(filename[0]) == target) {
		                currentSelection = static_cast<int>(i);
		                if (currentSelection < offset)
		                    offset = currentSelection;
		                else if (currentSelection >= offset + windowHeight - 2)
		                    offset = currentSelection - (windowHeight - 2) + 1;
		                draw(availableItems, selectedItems);
		                break;
		            }
		        }
		        continue;
		    }
		switch (ch) {
		case KEY_UP:
			if (currentSelection > 0) {
				--currentSelection;
				if (currentSelection < offset)
					--offset;
			}
			draw(availableItems, selectedItems);
			break;
		case KEY_DOWN:
			if (currentSelection < (int) availableItems.size() - 1) {
				++currentSelection;
				if (currentSelection >= offset + windowHeight - 2)
					++offset;
			}
			draw(availableItems, selectedItems);
			break;
		case KEY_PPAGE:
			currentSelection -= (windowHeight - 2);
			if (currentSelection < 0)
				currentSelection = 0;
			if (currentSelection < offset)
				offset = currentSelection;
			draw(availableItems, selectedItems);
			break;
		case KEY_NPAGE:
			currentSelection += (windowHeight - 2);
			if (currentSelection > (int) availableItems.size() - 1)
				currentSelection = (int) availableItems.size() - 1;
			if (currentSelection >= offset + windowHeight - 2)
				offset = currentSelection - (windowHeight - 2) + 1;
			draw(availableItems, selectedItems);
			break;
		case KEY_HOME:
			currentSelection = 0;
			offset = 0;
			draw(availableItems, selectedItems);
			break;
		case KEY_END:
			currentSelection = (int) availableItems.size() - 1;
			offset =
					(currentSelection >= windowHeight - 2) ?
							currentSelection - (windowHeight - 2) + 1 : 0;
			draw(availableItems, selectedItems);
			break;
		case ' ': {
			std::string chosen = availableItems[currentSelection];
			if (std::find(selectedItems.begin(), selectedItems.end(), chosen)
					== selectedItems.end()) {
				selectedItems.push_back(chosen);
			}
			draw(availableItems, selectedItems);
		}
			break;
		case 'A': // seleziona tutto in un colpo
		        if (allSelectable) {
		            selectedItems = availableItems;   // copia tutto
		            draw(availableItems, selectedItems);
		        }
		        break;
		case 10: // Enter
			if (selectedItems.size() == 0) {
				std::string chosen = availableItems[currentSelection];
				// controlla se il file non è già presente
				if (std::find(selectedItems.begin(), selectedItems.end(),
						chosen) == selectedItems.end()) {
					selectedItems.push_back(chosen);
				}
			}

			return selectedItems;
		case 27: // ESC
			return std::nullopt;
		}
	}
}

void ListSelectView::draw(const std::vector<std::string> &availableItems,
		const std::vector<std::string> &selectedItems) {
	clear();
	int maxDisplay = windowHeight - 2;
	mvprintw(0, 0, "%s", title.c_str());

	for (int i = 0; i < maxDisplay && i + offset < (int) availableItems.size();
			++i) {
		int index = i + offset;
		bool isSelected = std::find(selectedItems.begin(), selectedItems.end(),
				availableItems[index]) != selectedItems.end();

		if (index == currentSelection)
			attron(A_REVERSE);

		std::string filename =
				fs::path(availableItems[index]).filename().string();

		if (isSelected) {
			filename += " *";  // asterisco per item selezionati
		}

		mvprintw(i + 1, 2, "%s", filename.c_str());

		if (index == currentSelection)
			attroff(A_REVERSE);
	}
	refresh();
}
