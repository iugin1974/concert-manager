#pragma once
#include "Concert.h"
#include <vector>
#include <ncurses.h>

class SelectionView {

public:
	/**
	 * @brief Mostra una lista di oggetti di tipo generico e chiede all'utente di selezionarne uno.
	 *
	 * Il tipo T deve avere un metodo `std::string toString() const`.
	 *
	 * @tparam T Tipo dell'oggetto contenuto nel vettore.
	 * @param items Vettore di oggetti T da cui selezionare.
	 * @param label Etichetta da mostrare all’utente nella prompt.
	 * @return L’indice (zero-based) dell’oggetto selezionato, oppure -1 se vuoto, annullato, o errore.
	 */
	template<typename T>
	int runChoiceForm(const std::vector<T>& items, const std::string& label = "Choose an element:")
	{
	    clear();
	    refresh();

	    int l = items.size();
	    if (l == 0) {
	        mvprintw(0, 0, "No items present.");
	        refresh();
	        getch();
	        return -1;
	    }

	    for (int i = 0; i < l; ++i) {
	        mvprintw(i, 3, "%d. %s", i + 1, items[i].toString().c_str());
	    }

	    mvprintw(l + 2, 3, "%s", label.c_str());
	    int choice = promptNumber(stdscr, l + 3, 3, 1, l);

	    if (choice == 0 || choice < 1 || choice > l) {
	        mvprintw(l + 4, 3, "Invalid choice.");
	        refresh();
	        getch();
	        return -1;
	    }

	    return choice - 1;
	}

	template<typename T>
	std::string join(const std::vector<T>& vec, const std::string& sep) {
	    if (vec.empty()) return "";

	    std::string result = vec[0];
	    for (size_t i = 1; i < vec.size(); ++i) {
	        result += sep + vec[i];
	    }
	    return result;
	}

private:
	int promptNumber(WINDOW *win, int y, int x, int min, int max);
	void showErrorMessage(WINDOW* win, int y, int x, const std::string& message);
};
