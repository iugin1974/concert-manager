#include "Rehearsal.h"
#include <iomanip>
#include <sstream>

Rehearsal::Rehearsal() = default;

Rehearsal::Rehearsal(const std::tm& date,
                     const std::string& startTime,
                     const std::string& place,
                     const std::string& musicians)
    : date(date), startTime(startTime), place(place), musicians(musicians) {}

// Getters
std::tm Rehearsal::getDate() const { return date; }
std::string Rehearsal::getStartTime() const { return startTime; }
std::string Rehearsal::getPlace() const { return place; }
std::string Rehearsal::getMusicians() const { return musicians; }

// Setters
void Rehearsal::setDate(const std::tm& d) { date = d; }
void Rehearsal::setStartTime(const std::string& s) { startTime = s; }
void Rehearsal::setPlace(const std::string& p) { place = p; }
void Rehearsal::setMusicians(const std::string& m) { musicians = m; }

// toString
std::string Rehearsal::toString() const {
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d", &date);
    return std::string(buf) + ", " + startTime + " - " + place + " [" + musicians + "]";
}
