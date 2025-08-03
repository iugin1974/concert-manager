#pragma once

#include <string>
#include <ctime>
#include "Timestamped.h"
#include "Printable.h"

class Rehearsal : public Timestamped, Printable {
private:
    std::tm date{};              // solo parte data (YYYY-MM-DD)
    std::string startTime;       // orario in formato "HH:MM"
    std::string place;
    std::string musicians;

public:
    Rehearsal();
    Rehearsal(const std::tm& date,
              const std::string& startTime,
              const std::string& place,
              const std::string& musicians,
			  long long ts = 0);

    // Getters
    std::tm getDate() const;
    std::string getStartTime() const;
    std::string getPlace() const;
    std::string getMusicians() const;
    static std::string header();

    // Setters
    void setDate(const std::tm& d);
    void setStartTime(const std::string& s);
    void setPlace(const std::string& p);
    void setMusicians(const std::string& m);

    // Formattazione per output
    std::string toString() const;
};
