#include "MusicalPiece.h"
#include <iostream>

// Costruttore
MusicalPiece::MusicalPiece(const std::string& composer,
                           const std::string& title,
                           const std::string& key,
                           const std::string& duration,
                           bool choir,
                           const std::string& singerPart,
                           const std::string& instruments)
    : composer(composer), title(title), key(key), duration(duration),
      choir(choir), singerPart(singerPart), instruments(instruments) {}

// Getter
std::string MusicalPiece::getComposer() const    { return composer; }
std::string MusicalPiece::getTitle() const       { return title; }
std::string MusicalPiece::getKey() const         { return key; }
std::string MusicalPiece::getDuration() const    { return duration; }
bool MusicalPiece::hasChoir() const              { return choir; }
std::string MusicalPiece::getSingerPart() const  { return singerPart; }
std::string MusicalPiece::getInstruments() const { return instruments; }

// Setter
void MusicalPiece::setComposer(const std::string& c)      { composer = c; }
void MusicalPiece::setTitle(const std::string& t)         { title = t; }
void MusicalPiece::setKey(const std::string& k)           { key = k; }
void MusicalPiece::setDuration(const std::string& d)      { duration = d; }
void MusicalPiece::setChoir(bool ch)                      { choir = ch; }
void MusicalPiece::setSingerPart(const std::string& s)    { singerPart = s; }
void MusicalPiece::setInstruments(const std::string& i)   { instruments = i; }

// Print
void MusicalPiece::print() const {
    std::cout << "Title: " << title << "\n"
              << "Composer: " << composer << "\n"
              << "Key: " << key << "\n"
              << "Duration: " << duration << "\n"
              << "Choir: " << (choir ? "Yes" : "No") << "\n"
              << "Singer Part: " << singerPart << "\n"
              << "Instruments: " << instruments << "\n";
}
