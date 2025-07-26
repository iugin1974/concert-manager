#include "Concert.h"

#include <sstream>
#include <iomanip>
#include <algorithm>
#include <ctime>

Concert::Concert() : title("Untitled") {}

Concert::Concert(const std::string &t) : title(t) {}

void Concert::setTitle(const std::string &t) {
    title = t;
}

const std::string &Concert::getTitle() const {
    return title;
}

void Concert::setPlaces(const std::vector<std::string> &p) {
    places = p;
}

const std::vector<std::string> &Concert::getPlaces() const {
    return places;
}

std::vector<std::string> &Concert::getPlaces() {
    return places;
}

void Concert::addDate(const std::string &d) {
    std::tm date{};
    if (stringToTm(d, date)) {
        dates.push_back(date);
        std::sort(dates.begin(), dates.end(), compareTm);
    }
    // else gestione errore (opzionale)
}

void Concert::setDatesAsString(const std::vector<std::string> &d) {
    dates.clear();
    for (const auto &dateStr : d) {
        std::tm date{};
        if (stringToTm(dateStr, date)) {
            dates.push_back(date);
        }
    }
    std::sort(dates.begin(), dates.end(), compareTm);
}

std::vector<std::string> Concert::getDatesAsString() const {
    std::vector<std::string> result;
    for (const auto &date : dates) {
        result.push_back(tmToString(date));
    }
    return result;
}

const std::vector<std::tm> &Concert::getDatesAsTm() const {
    return dates;
}

std::vector<std::tm> &Concert::getDatesAsTm() {
    return dates;
}

void Concert::setProgram(const std::vector<MusicalPiece> &p) {
    program = p;
}

const std::vector<MusicalPiece> &Concert::getProgram() const {
    return program;
}

std::vector<MusicalPiece> &Concert::getProgram() {
    return program;
}

void Concert::setMusicians(const std::vector<Musician> &m) {
    musicians = m;
}

void Concert::addMusician(const Musician &m) {
    musicians.push_back(m);
}

const std::vector<Musician> &Concert::getMusicians() const {
    return musicians;
}

std::vector<Musician> &Concert::getMusicians() {
    return musicians;
}

bool Concert::hasMusicians() const {
    return !musicians.empty();
}

void Concert::addRehearsal(const Rehearsal &r) {
    rehearsals.push_back(r);
}

void Concert::setRehearsals(const std::vector<Rehearsal> &r) {
    rehearsals = r;
}

const std::vector<Rehearsal> &Concert::getRehearsals() const {
    return rehearsals;
}

std::vector<Rehearsal> &Concert::getRehearsals() {
    return rehearsals;
}

void Concert::setToDo(const std::string &td) {
    todo = td;
}

const std::string &Concert::getToDo() const {
    return todo;
}

void Concert::setComment(const std::string &c) {
    comment = c;
}

const std::string &Concert::getComment() const {
    return comment;
}

int Concert::getDuration() const {
    int duration = 0;
    for (const auto &piece : program) {
        duration += piece.getDuration();
    }
    return duration;
}

std::string Concert::tmToString(const std::tm &date) {
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << date.tm_mday << "."
        << std::setw(2) << (date.tm_mon + 1) << "."
        << (date.tm_year + 1900);
    return oss.str();
}

bool Concert::stringToTm(const std::string &dateStr, std::tm &date) {
    std::istringstream iss(dateStr);
    char dot1, dot2;
    int day, month, year;

    if (!(iss >> day >> dot1 >> month >> dot2 >> year))
        return false;
    if (dot1 != '.' || dot2 != '.')
        return false;

    date = {};
    date.tm_mday = day;
    date.tm_mon = month - 1;
    date.tm_year = year - 1900;

    // Verifica validità con mktime
    std::tm temp = date;
    if (mktime(&temp) == -1)
        return false;

    return true;
}

bool Concert::compareTm(const std::tm &a, const std::tm &b) {
    if (a.tm_year != b.tm_year)
        return a.tm_year < b.tm_year;
    if (a.tm_mon != b.tm_mon)
        return a.tm_mon < b.tm_mon;
    return a.tm_mday < b.tm_mday;
}

std::string Concert::toString() const {
    return title;
}
