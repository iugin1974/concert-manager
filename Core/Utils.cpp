#include <ncurses.h>
#include <string>
#include <cctype>
#include <thread>
#include <chrono>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>

std::string trim(const char* buffer) {
    std::string str(buffer);

    // Rimuove spazi finali e newline
    size_t end = str.find_last_not_of(" \n");
    if (end != std::string::npos)
        str.erase(end + 1);
    else
        str.clear(); // Solo spazi → svuota tutta la stringa

    // Rimuove spazi iniziali
    size_t start = str.find_first_not_of(" ");
    if (start != std::string::npos)
        str.erase(0, start);

    return str;
}

// Converte std::tm in stringa "DD.MM.YYYY"
std::string dateToString(const std::tm& date) {
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << date.tm_mday << "."
        << std::setw(2) << date.tm_mon + 1 << "."  // tm_mon è 0-based
        << std::setw(4) << date.tm_year + 1900;   // tm_year è years since 1900
    return oss.str();
}

// Converte stringa "DD.MM.YYYY" in std::tm
bool stringToDate(const std::string& str, std::tm& result) {
    if (str.length() != 10 || str[2] != '.' || str[5] != '.')
        return false;

    int day, month, year;
    try {
        day = std::stoi(str.substr(0, 2));
        month = std::stoi(str.substr(3, 2));
        year = std::stoi(str.substr(6, 4));
    } catch (...) {
        return false;
    }

    if (day < 1 || day > 31 || month < 1 || month > 12)
        return false;

    result = {};
    result.tm_mday = day;
    result.tm_mon = month - 1;     // tm_mon is 0-based
    result.tm_year = year - 1900;  // tm_year is years since 1900

    return true;
}

bool confirmDialog(WINDOW* parent) {
    std::string message = "Are you sure (Y/n)?";
    int width = message.size() + 4;
    int height = 5;

    int posY = (LINES - height) / 2;
    int posX = (COLS - width) / 2;

    WINDOW* dialog_win = newwin(height, width, posY, posX);
    box(dialog_win, 0, 0);
    mvwprintw(dialog_win, 2, 2, "%s", message.c_str());
    wrefresh(dialog_win);

    keypad(dialog_win, TRUE);
    int ch;
    bool result = false;

    while (true) {
        ch = wgetch(dialog_win);
        if (ch == 'Y' || ch == 'y') {
            result = true;
            break;
        } else if (ch == 'N' || ch == 'n' || ch == 27) { // ESC as no
            result = false;
            break;
        }
    }

    werase(dialog_win);
    wrefresh(dialog_win);
    delwin(dialog_win);
    touchwin(parent);
    wrefresh(parent);
    return result;
}


std::string intToString(int value) {
    return std::to_string(value);
}

int stringToInt(const std::string& str) {
    try {
        size_t pos;
        int result = std::stoi(str, &pos);
        if (pos != str.length()) {
            throw std::invalid_argument("La stringa contiene caratteri non validi.");
        }
        return result;
    } catch (const std::exception& e) {
        throw std::invalid_argument("Conversione fallita: " + std::string(e.what()));
    }
}


std::string convertToMMSS(int totalSeconds) {
    if (totalSeconds < 0) {
        throw std::invalid_argument("I secondi devono essere >= 0");
    }

    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << minutes
        << ":" << std::setw(2) << std::setfill('0') << seconds;

    return oss.str();
}

// ritorna -1 se la conversione non è possibile
int convertToSeconds(const std::string& timeStr) {
    int minutes = 0, seconds = 0;
    char colon;

    std::istringstream iss(timeStr);
    if (!(iss >> minutes >> colon >> seconds) || colon != ':' || seconds < 0 || seconds >= 60 || minutes < 0) {
return -1;      
//  throw std::invalid_argument("Formato non valido. Usa MM:SS con minuti >= 0 e secondi tra 0 e 59.");
    }

    return minutes * 60 + seconds;
}

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
