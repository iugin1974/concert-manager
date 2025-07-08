#include <ncurses.h>
#include <string>
#include <cctype>
#include <thread>
#include <chrono>


// Mostra un messaggio temporaneo in rosso per errori
void showErrorMessage(WINDOW* win, int y, int x, const std::string& message) {
    wattron(win, A_BOLD | COLOR_PAIR(3));  // Assumi che COLOR_PAIR(3) sia rosso
    mvwprintw(win, y + 1, x, "%s", message.c_str());
    wattroff(win, A_BOLD | COLOR_PAIR(3));
    wrefresh(win);
    beep();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    mvwprintw(win, y + 1, x, "%*s", static_cast<int>(message.size()), " ");  // Pulisce
    wrefresh(win);
}

int promptNumber(WINDOW* win, int y, int x, int min, int max) {
    std::string prompt = "(" + std::to_string(min) + "-" + std::to_string(max) + ") > ";
    std::string input;
    int ch;

    int input_x = x + prompt.length();

    while (true) {
        mvwprintw(win, y, x, "%s", prompt.c_str());
        wmove(win, y, input_x);
        wrefresh(win);

        input.clear();

        while (true) {
            ch = wgetch(win);

            if (ch == 27) return -1;  // ESC = Annulla

            else if (ch == '\n' || ch == KEY_ENTER) {
                if (!input.empty()) {
                    int num = std::stoi(input);
                    if (num >= min && num <= max)
                        return num;
                }
                showErrorMessage(win, y, x, "Input non valido!");
                break;  // Resetta
            }

            else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
                if (!input.empty()) {
                    input.pop_back();
mvwprintw(win, y, input_x, "%*s", static_cast<int>(input.length() + 1), " ");
                    mvwprintw(win, y, input_x, "%s", input.c_str());
                    wmove(win, y, input_x + input.length());
                    wrefresh(win);
                }
            }

            else if (isdigit(ch)) {
                input.push_back(ch);
                mvwprintw(win, y, input_x, "%s", input.c_str());
                wmove(win, y, input_x + input.length());
                wrefresh(win);
            }
        }

        // Cancella riga
        mvwprintw(win, y, x, "%*s", static_cast<int>(prompt.length() + input.length()), " ");
    }
}
