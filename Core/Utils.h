#pragma once
#include <ncurses.h>
#include <vector>
#include <ctime>

int print(int line, int col, const char *fmt, ...);
std::string trim(const char* buffer);
std::string formatGermanDate(const std::tm &date);
std::string dateToString(const std::tm& date);
bool stringToDate(const std::string& str, std::tm& result);
bool confirmDialog(WINDOW* parent);
int promptNumber(WINDOW* win, int y, int x, int min, int max);
int convertToSeconds(const std::string& timeStr);
std::string convertToMMSS(int totalSeconds);
int stringToInt(const std::string& str);
std::string intToString(int value);
