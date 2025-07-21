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
        endwin();

        // 3. Fork + exec dell’editor
        pid_t pid = fork();
        if (pid == 0) {
            execlp("vim", "vim", tmpName, (char *)nullptr);
            _exit(1); // execlp fallito
        } else if (pid > 0) {
            int status;
            if (waitpid(pid, &status, 0) == -1 ||
                !WIFEXITED(status) || WEXITSTATUS(status) != 0) {
                std::remove(tmpName);
                refresh(); // Ripristina ncurses
                return false;
            }
        } else {
            std::remove(tmpName);
            refresh();
            return false; // Fork fallito
        }

        // 4. Riattiva ncurses
        refresh();

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
