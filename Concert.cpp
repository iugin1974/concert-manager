#include "Concert.h"
#include "Musician.h"       
#include "MusicalPiece.h"
#include "ctime"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>  // per std::sort
Concert::Concert() : title("Untitled") {}

Concert::Concert(const std::string& t) : title(t) {}

void Concert::setTitle(const std::string& t) {
    title = t;
}

const std::string& Concert::getTitle() const {
    return title;
}

void Concert::setPlaces(const std::vector<std::string>& p) {
    places = p;
}

const std::vector<std::string>& Concert::getPlaces() const {
    return places;
}

void Concert::addDate(std::string& d) {
	tm date;
stringToTm(d, date);
    dates.push_back(date);
}

void Concert::setDatesAsString(const std::vector<std::string>& d) {
    dates.clear();
    for (const auto& dateStr : d) {
        tm date;
        if (Concert::stringToTm(dateStr, date)) {
            dates.push_back(date);
        } else {
            // Gestione errore: puoi decidere cosa fare in caso di data non valida
            // Per esempio ignorare, oppure inserire una data "nulla"
        }
    }
std::sort(dates.begin(), dates.end(), compareTm);
}


std::vector<std::string> Concert::getDatesAsString() const {
    std::vector<std::string> result;
    for (const auto& date : dates) {
        result.push_back(Concert::tmToString(date));
    }
    return result;
}

const std::vector<tm>& Concert::getDatesAsTm() const {
    return dates;
}

void Concert::setProgram(const std::vector<MusicalPiece>& p) {
    program = p;
}

void Concert::setMusicians(const std::vector<Musician>& m) {
    musicians = m;
}

void Concert::addMusician(const Musician& m) {
    musicians.push_back(m);
}

std::vector<MusicalPiece> Concert::getProgram() const {
    return program;
}

std::vector<Musician> Concert::getMusicians() const {
    return musicians;
}

bool Concert::hasMusicians() const {
    return musicians.size() > 0;
}

void Concert::setToDo(const std::string& td) {
todo = td;
}

const std::string Concert::getToDo() const {
return todo;
} 

int Concert::getDuration() const {
int duration = 0;
for (size_t i = 0; i<program.size(); i++) {
duration += program.at(i).getDuration();
}
return duration;
}

// Converte tm in stringa "gg.mm.aaaa"
std::string Concert::tmToString(const std::tm& date) {
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << date.tm_mday << "."
        << std::setw(2) << (date.tm_mon + 1) << "."
        << (date.tm_year + 1900);
    return oss.str();
}

// Converte stringa "gg.mm.aaaa" in tm
// Ritorna true se la conversione è andata a buon fine, false altrimenti
bool Concert::stringToTm(const std::string& dateStr, std::tm& date) {
    std::istringstream iss(dateStr);
    char dot1, dot2;
    int day, month, year;

    if (!(iss >> day >> dot1 >> month >> dot2 >> year))
        return false;

    if (dot1 != '.' || dot2 != '.')
        return false;

    // Valori base per tm
    date = {};
    date.tm_mday = day;
    date.tm_mon = month - 1;    // tm_mon va da 0 a 11
    date.tm_year = year - 1900; // tm_year è anni dal 1900

    // Controllo di validità basilare (non completo)
    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900)
        return false;

    return true;
}

// Funzione di confronto per tm
bool Concert::compareTm(const std::tm& a, const std::tm& b) {
    if (a.tm_year != b.tm_year)
        return a.tm_year < b.tm_year;
    if (a.tm_mon != b.tm_mon)
        return a.tm_mon < b.tm_mon;
    return a.tm_mday < b.tm_mday;
}
