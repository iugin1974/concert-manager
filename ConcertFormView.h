// ConcertFormView.h
#pragma once
#include "Concert.h"
#include <string>
#include <vector>

class ConcertFormView {
public:
ConcertFormView(); 
ConcertFormView(const Concert& c);
    void show(const Concert* existing = nullptr);

    std::string getTitle() const;
    std::vector<std::string> getPlaces() const;
    std::vector<std::string> getDates() const;
    std::vector<Musician> getMusicians() const;

private:
    std::string title;
    std::vector<std::string> places;
    std::vector<std::string> dates;
    std::vector<Musician> musicians;
};

