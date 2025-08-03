#ifndef MUSICALPIECE_H
#define MUSICALPIECE_H

#include <string>
#include <vector>

#include "Printable.h"
#include "Score.h"
#include "Timestamped.h"

class MusicalPiece: public Timestamped, Printable {
private:
	std::string composer;
	std::string title;
	int duration = 0;  // sec.
	bool choir = false;
	std::string singerPart;
	std::string instruments;  // es. "Vl1, Vl2, Vla, Kb"
	std::vector<Score> scores;
	std::string youtubeLink;

public:
	// Costruttore
	MusicalPiece();
	MusicalPiece(const std::string &composer, const std::string &title,
	             int duration, bool choir, const std::string &singerPart,
	             const std::string &instruments, const std::string &youtubeLink = "", long long ts = 0);

	// Getter
	const std::string& getComposer() const;
	const std::string& getTitle() const;
	int getDuration() const;
	bool hasChoir() const;
	const std::string& getSingerPart() const;
	const std::string& getInstruments() const;

	// Setter
	void setComposer(const std::string &composer);
	void setTitle(const std::string &title);
	void setDuration(const int duration);
	void setChoir(bool choir);
	void setSingerPart(const std::string &singerPart);
	void setInstruments(const std::string &instruments);
	const std::vector<Score>& getScores() const;
		std::vector<Score>& getScores();

		void setScores(const std::vector<Score> &s);
		void addScore(const Score &s);
		const std::string& getYoutubeLink() const;
		void setYoutubeLink(const std::string &link);
	// Stampa a terminale
	void print() const;
	std::string toString() const override;
	static std::string header();
};

#endif // MUSICALPIECE_H
