#include "MusicalPiece.h"
#include <iostream>

// Costruttore
MusicalPiece::MusicalPiece() { choir = false; }
MusicalPiece::MusicalPiece(const std::string& composer,
                           const std::string& title,
                           int duration,
                           bool choir,
                           const std::string& singerPart,
                           const std::string& instruments,
                           long long ts)
    : Timestamped(ts),
	  composer(composer),
      title(title),
      duration(duration),
      choir(choir),
      singerPart(singerPart),
      instruments(instruments) {}


// Getter
const std::string& MusicalPiece::getComposer() const    { return composer; }
const std::string& MusicalPiece::getTitle() const       { return title; }
int MusicalPiece::getDuration() const    { return duration; }
bool MusicalPiece::hasChoir() const              { return choir; }
const std::string& MusicalPiece::getSingerPart() const  { return singerPart; }
const std::string& MusicalPiece::getInstruments() const { return instruments; }

// Setter
void MusicalPiece::setComposer(const std::string& c)      { composer = c; }
void MusicalPiece::setTitle(const std::string& t)         { title = t; }
void MusicalPiece::setDuration(const int d)      { duration = d; }
void MusicalPiece::setChoir(bool ch)                      { choir = ch; }
void MusicalPiece::setSingerPart(const std::string& s)    { singerPart = s; }
void MusicalPiece::setInstruments(const std::string& i)   { instruments = i; }

// Print
void MusicalPiece::print() const {
    std::cout << "Title: " << title << "\n"
              << "Composer: " << composer << "\n"
              << "Duration: " << duration << "\n"
              << "Choir: " << (choir ? "Yes" : "No") << "\n"
              << "Singer Part: " << singerPart << "\n"
              << "Instruments: " << instruments << "\n";
}

std::string MusicalPiece::toString() const {
return composer + ": " + title;
}
