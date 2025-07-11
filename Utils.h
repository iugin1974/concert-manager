#pragma once
#include <ncurses.h>

int promptNumber(WINDOW* win, int y, int x, int min, int max);
int convertToSeconds(const std::string& timeStr);
std::string convertToMMSS(int totalSeconds);
int stringToInt(const std::string& str);
std::string intToString(int value);