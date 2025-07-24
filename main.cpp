#include "ConcertController.h"
#include "logMessage.h"
#include <vector>
#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cstdlib>
#include "FileIO.h"
#include <signal.h>

std::string getHomePath() {
	const char *home = std::getenv("HOME");
	return home ? std::string(home) : ".";
}

void load_config() {
	const char *home = std::getenv("HOME");
	if (!home) {
		std::cerr << "Error: HOME environment variable not set.\n";
		std::exit(1);
	}

	std::filesystem::path config_path = std::filesystem::path(home)
			/ ".concertmanagerrc";

	if (!std::filesystem::exists(config_path)) {
		std::cerr << "[concertmanager] Configuration file not found:\n";
		std::cerr << "Please create a file at: " << config_path << "\n";
		std::cerr << "Example content:\n";
		std::cerr << "scoresBasePath=/your/path/to/partiture\n";
		std::cerr << "savePath=/your/path/for/xml\n";
		std::exit(1);
	}

	std::ifstream infile(config_path);
	std::string line;

	while (std::getline(infile, line)) {
		if (line.rfind("scoresBasePath=", 0) == 0) {
			Score::basePathScores = line.substr(
					std::string("scoresBasePath=").length());
			LOG_MSG("ScoresBasePath is " + Score::basePathScores);
		} else if (line.rfind("savePath=", 0) == 0) {
			FileIO::savePath = line.substr(std::string("savePath=").length());
			LOG_MSG("SavePath is " + FileIO::savePath);
		}
	}

	// Fallback defaults
	if (FileIO::savePath.empty()) {
		FileIO::savePath = getHomePath();
	}

	// Validate paths
	if (Score::basePathScores.empty()) {
		std::cerr
				<< "[concertmanager] Error: 'scoresBasePath' is missing in .concertmanagerrc.\n";
		std::exit(1);
	}
	if (!std::filesystem::exists(Score::basePathScores)
			|| !std::filesystem::is_directory(Score::basePathScores)) {
		std::cerr << "[concertmanager] Error: scoresBasePath '"
				<< Score::basePathScores
				<< "' in .concertmanagerrc does not exist or is not a directory.\n";
		std::exit(1);
	}

	if (!std::filesystem::exists(FileIO::savePath)
			|| !std::filesystem::is_directory(FileIO::savePath)) {
		std::cerr << "[concertmanager] Error: savePath '" << FileIO::savePath
				<< "' in .concertmanagerrc does not exist or is not a directory.\n";
		std::exit(1);
	}
}

// Handler per segnali di crash
void handle_crash(int sig) {
    endwin();  // Ripristina il terminale
    fprintf(stderr, "\n[Caught signal %d, restoring terminal before crash]\n", sig);
    // Rilancia il segnale per permettere ad ASan di fare il suo lavoro
    signal(sig, SIG_DFL);
    raise(sig);
}

int main() {
	 // Imposta il crash handler per i segnali più comuni
	    signal(SIGSEGV, handle_crash);
	    signal(SIGABRT, handle_crash);
	    signal(SIGFPE,  handle_crash);
	    signal(SIGILL,  handle_crash);
	    signal(SIGBUS,  handle_crash);

	load_config();
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
