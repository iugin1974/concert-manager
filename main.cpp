#include "ConcertController.h"

#include <vector>
#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cstdlib>

std::string get_partiture_base_path() {
    const char* home = std::getenv("HOME");
    if (!home) {
        std::cerr << "Error: HOME environment variable not set.\n";
        return "";
    }

    std::filesystem::path config_path = std::filesystem::path(home) / ".concertmanagerrc";

    if (!std::filesystem::exists(config_path)) {
        std::cerr << "[concertmanager] Configuration file not found:\n";
        std::cerr << "Please create a file at: " << config_path << "\n";
        std::cerr << "The file should contain one line like:\n";
        std::cerr << "path=/your/path/to/partiture\n";
        return "";
    }

    std::ifstream infile(config_path);
    std::string line;
    while (std::getline(infile, line)) {
        if (line.rfind("path=", 0) == 0) { // starts with "path="
            std::string path = line.substr(5); // remove "path="
            return path;
        }
    }

    std::cerr << "[concertmanager] Invalid config file format.\n";
    std::cerr << "Expected line like: path=/your/path/to/partiture\n";
    return "";
}

int main()
{
	std::string base_path = get_partiture_base_path();
	if (base_path.empty()) {
	    exit(1);
	}
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
