#include "Concert.h"
#include "Musician.h"       
#include "MusicalPiece.h"
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

void Concert::setDates(const std::vector<std::string>& d) {
    dates = d;
}

const std::vector<std::string>& Concert::getDates() const {
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