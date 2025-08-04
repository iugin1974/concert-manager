#include "Rehearsal.h"
#include "Utils.h"
#include <iomanip>
#include <sstream>

Rehearsal::Rehearsal() : Timestamped() {}

Rehearsal::Rehearsal(const std::tm &date,
                     const std::string &startTime,
                     const std::string &place,
                     const std::string &musicians,
                     long long ts)
    : Timestamped(ts == 0 ? generateTimestamp() : ts),
      date(date),
      startTime(startTime),
      place(place),
      musicians(musicians)
{}


// Getters
std::tm Rehearsal::getDate() const {
	return date;
}
std::string Rehearsal::getStartTime() const {
	return startTime;
}
std::string Rehearsal::getPlace() const {
	return place;
}
std::string Rehearsal::getMusicians() const {
	return musicians;
}

// Setters
void Rehearsal::setDate(const std::tm &d) {
	date = d;
}
void Rehearsal::setStartTime(const std::string &s) {
	startTime = s;
}
void Rehearsal::setPlace(const std::string &p) {
	place = p;
}
void Rehearsal::setMusicians(const std::string &m) {
	musicians = m;
}

// toString
std::string Rehearsal::toString() const {
        std::ostringstream oss;
        oss << std::left
            << std::setw(12) << dateToString(getDate())
            << std::setw(6)  << getStartTime()
            << std::setw(30) << getPlace()
            << std::setw(20) << getMusicians();
        return oss.str();
    }

    std::string Rehearsal::header() {
        std::ostringstream oss;
        oss << std::left
            << std::setw(12) << "Date"
            << std::setw(6)  << "Time"
            << std::setw(30) << "Place"
            << std::setw(20) << "Musicians";
        return oss.str();
    }
