#pragma once

#include <string>
#include <vector>
#include "Musician.h"      
#include "MusicalPiece.h"
class Concert {
public:
    Concert();  // costruttore di default
    Concert(const std::string& title);

    void setTitle(const std::string& t);
    const std::string& getTitle() const;

    void setPlaces(const std::vector<std::string>& p);
    const std::vector<std::string>& getPlaces() const;

    void setDates(const std::vector<std::string>& d);
    const std::vector<std::string>& getDates() const;

void setProgram(const std::vector<MusicalPiece>& p);
void setMusicians(const std::vector<Musician>& m);
void addMusician(const Musician& m);

std::vector<MusicalPiece> getProgram() const;
std::vector<Musician> getMusicians() const;

bool hasMusicians() const;

void setComment(const std::string& c) { comment = c; }
    std::string getComment() const { return comment; }


private:
    std::string title;
    std::vector<std::string> places;
    std::vector<std::string> dates;
	std::vector<MusicalPiece> program;  
    std::vector<Musician> musicians; 
std::string comment;
};
