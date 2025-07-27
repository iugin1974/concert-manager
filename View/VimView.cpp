#include "VimView.h"
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <ncurses.h>

bool VimView::edit(const std::string &text) {
        // 1. Crea un file temporaneo sicuro
        char tmpName[] = "/tmp/editor_XXXXXX";
        int fd = mkstemp(tmpName);
        if (fd == -1)
            return false;

        // Scrivi il contenuto iniziale nel file
        {
            std::ofstream out(tmpName);
            if (!out) {
                close(fd);
                std::remove(tmpName);
                return false;
            }
            out << text;
        }

        close(fd); // Non ci serve più il file descriptor

        // 2. Sospendi ncurses prima di lanciare Vim
        def_prog_mode();   // Salva lo stato corrente di ncurses
        endwin();          // Sospende ncurses

        // 3. Lancia Vim con system()
        std::string command = "vim " + std::string(tmpName);
        int status = system(command.c_str());
        if (status != 0) {
            std::remove(tmpName);
            refresh();
            return false;
        }

        // 4. Riattiva ncurses
        reset_prog_mode(); // Ripristina modalità ncurses
        refresh();         // Ridisegna

        // 5. Leggi il contenuto modificato
        {
            std::ifstream in(tmpName);
            if (!in) {
                std::remove(tmpName);
                return false;
            }

            std::string line;
            while (std::getline(in, line)) {
            	newText += line;
            	newText += '\n';
            }

            if (!newText.empty() && newText.back() == '\n')
            	newText.pop_back();
        }

        // 6. Rimuovi il file temporaneo
        std::remove(tmpName);
        return true;
    }

std::string VimView::getText() {
	return newText;
}
