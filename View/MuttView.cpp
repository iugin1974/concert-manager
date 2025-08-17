#include "MuttView.h"
#include <ncurses.h>

void MuttView::launchMutt(const std::string &args) {
    def_prog_mode();
    endwin();

    std::string command = "mutt " + args + " --";
    system(command.c_str());

    reset_prog_mode();
    refresh();
}
