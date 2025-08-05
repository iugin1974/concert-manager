#include "Concert.h"
#include "logMessage.h"
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

void Concert::addStartTime(const std::string &t) {
    std::tm time {};
    if (stringToTm(t, time)) {
        startTimes.push_back(time);
    }
}

void Concert::setStartTimesAsString(const std::vector<std::string> &t) {
    startTimes.clear();
    for (const auto &s : t) {
        std::tm time{};
        if (stringToHourMin(s, time)) {
            startTimes.push_back(time);
        } else {
            LOG_MSG("Invalid startTime format: " + s);
        }
    }
}

std::string Concert::tmToHourMinString(const std::tm &t) {
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << t.tm_hour
        << ":" << std::setw(2) << std::setfill('0') << t.tm_min;
    return oss.str();
}


std::vector<std::string> Concert::getStartTimesAsString() const {
    std::vector<std::string> result;
    for (const auto &t : startTimes) {
        result.push_back(tmToHourMinString(t));
    }
    return result;
}

const std::vector<std::tm>& Concert::getStartTimesAsTm() const {
    return startTimes;
}

std::vector<std::tm>& Concert::getStartTimesAsTm() {
    return startTimes;
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

bool Concert::stringToHourMin(const std::string &timeStr, std::tm &time) {
    std::istringstream iss(timeStr);
    char colon;
    int hour, minute;

    if (!(iss >> hour >> colon >> minute))
        return false;
    if (colon != ':')
        return false;
    if (hour < 0 || hour > 23 || minute < 0 || minute > 59)
        return false;

    time = {};
    time.tm_hour = hour;
    time.tm_min = minute;
    return true;
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

std::string Concert::header() {
    std::ostringstream oss;
    oss << std::left
        << std::setw(30) << "Title"
        << std::setw(30) << "Places"
        << std::setw(20) << "Dates"
        << std::setw(15) << "Start Times";
    return oss.str();
}

std::string Concert::toString() const {
    std::ostringstream oss;

    // Unisci places
    std::string placeStr;
    for (size_t i = 0; i < places.size(); ++i)
        placeStr += (i ? ", " : "") + places[i];

    // Unisci dates
    std::string dateStr;
    for (size_t i = 0; i < dates.size(); ++i)
        dateStr += (i ? ", " : "") + tmToString(dates[i]);

    // Unisci startTimes
    std::string startTimeStr;
    for (size_t i = 0; i < startTimes.size(); ++i)
        startTimeStr += (i ? ", " : "") + tmToString(startTimes[i]);

    oss << std::left
        << std::setw(30) << title
        << std::setw(30) << placeStr
        << std::setw(20) << dateStr
        << std::setw(15) << startTimeStr;

    return oss.str();
}
