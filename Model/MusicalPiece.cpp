#include "MusicalPiece.h"
#include "Score.h"
#include "Utils.h"
#include <iostream>
#include <vector>

// Costruttore
MusicalPiece::MusicalPiece() {
	choir = false;
}
MusicalPiece::MusicalPiece(const std::string &composer,
                           const std::string &title,
                           int duration,
                           bool choir,
                           const std::string &singerPart,
                           const std::string &instruments,
						   const std::string &youtubeLink,
                           long long ts)
    : Timestamped(ts),
      composer(composer),
      title(title),
      duration(duration),
      choir(choir),
      singerPart(singerPart),
      instruments(instruments),
      youtubeLink(youtubeLink) {
}
// Getter
const std::string& MusicalPiece::getComposer() const {
	return composer;
}
const std::string& MusicalPiece::getTitle() const {
	return title;
}
int MusicalPiece::getDuration() const {
	return duration;
}
bool MusicalPiece::hasChoir() const {
	return choir;
}
const std::string& MusicalPiece::getSingerPart() const {
	return singerPart;
}
const std::string& MusicalPiece::getInstruments() const {
	return instruments;
}

// Setter
void MusicalPiece::setComposer(const std::string &c) {
	composer = c;
}
void MusicalPiece::setTitle(const std::string &t) {
	title = t;
}
void MusicalPiece::setDuration(const int d) {
	duration = d;
}
void MusicalPiece::setChoir(bool ch) {
	choir = ch;
}
void MusicalPiece::setSingerPart(const std::string &s) {
	singerPart = s;
}
void MusicalPiece::setInstruments(const std::string &i) {
	instruments = i;
}
const std::vector<Score>& MusicalPiece::getScores() const {
	return scores;
}
std::vector<Score>& MusicalPiece::getScores() {
	return scores;
}

void MusicalPiece::setScores(const std::vector<Score> &s) {
	scores = s;
}
void MusicalPiece::addScore(const Score &s) {
	scores.push_back(s);
}

const std::string& MusicalPiece::getYoutubeLink() const {
    return youtubeLink;
}

void MusicalPiece::setYoutubeLink(const std::string &link) {
    youtubeLink = link;
}

// Print
void MusicalPiece::print() const {
	std::cout << "Title: " << title << "\n" << "Composer: " << composer << "\n"
			<< "Duration: " << duration << "\n" << "Choir: "
			<< (choir ? "Yes" : "No") << "\n" << "Singer Part: " << singerPart
			<< "\n" << "Instruments: " << instruments << "\n";
}

std::string MusicalPiece::toString() const {
        std::ostringstream oss;
        oss << std::left
			<< std::setw(40) << getTitle()
            << std::setw(20) << getComposer()
			<< std::setw(12)  << convertToMMSS(getDuration());

        // Costruzione campo Instruments (con SingerPart e Choir)
        std::ostringstream org;
        org << getInstruments();
        if (!getSingerPart().empty())
            org << " - " << getSingerPart();
        if (hasChoir())
            org << " - Choir";

        oss << std::setw(40) << org.str()
            << std::setw(3) << getScores().size();

        return oss.str();
    }

std::string MusicalPiece::header() {
        std::ostringstream oss;
        oss << std::left
		<< std::setw(40) << "Title"
            << std::setw(20) << "Composer"
            << std::setw(12)  << "Duration"
            << std::setw(40) << "Instruments"
            << std::setw(3)  << "Scores";
        return oss.str();
    }

