#ifndef CONCERT_H
#define CONCERT_H

#include <string>
#include <vector>
#include <ctime>
#include "Musician.h"
#include "MusicalPiece.h"
#include "Rehearsal.h"
#include "Score.h"

class Concert: public Timestamped {
private:
	std::string title;
	std::vector<std::string> places;
	std::vector<std::tm> dates;
	std::vector<MusicalPiece> program;
	std::vector<Musician> musicians;
	std::vector<Rehearsal> rehearsals;
	std::string todo;
	std::string comment;

public:

	Concert();
	Concert(const std::string &t);

	void setTitle(const std::string &t);
	const std::string& getTitle() const;

	void setPlaces(const std::vector<std::string> &p);
	const std::vector<std::string>& getPlaces() const;
	std::vector<std::string>& getPlaces();  // overload non const

	void addDate(const std::string &d);
	void setDatesAsString(const std::vector<std::string> &d);
	std::vector<std::string> getDatesAsString() const;

	const std::vector<std::tm>& getDatesAsTm() const;
	std::vector<std::tm>& getDatesAsTm();  // overload non const

	void setProgram(const std::vector<MusicalPiece> &p);
	const std::vector<MusicalPiece>& getProgram() const;
	std::vector<MusicalPiece>& getProgram();  // overload non const

	void setMusicians(const std::vector<Musician> &m);
	void addMusician(const Musician &m);
	const std::vector<Musician>& getMusicians() const;
	std::vector<Musician>& getMusicians();  // overload non const

	bool hasMusicians() const;

	void addRehearsal(const Rehearsal &r);
	void setRehearsals(const std::vector<Rehearsal> &r);
	const std::vector<Rehearsal>& getRehearsals() const;
	std::vector<Rehearsal>& getRehearsals();  // overload non const

	void setToDo(const std::string &td);
	const std::string& getToDo() const;

	void setComment(const std::string &c);
	const std::string& getComment() const;

	int getDuration() const;

	static std::string tmToString(const std::tm &date);
	static bool stringToTm(const std::string &dateStr, std::tm &date);
	static bool compareTm(const std::tm &a, const std::tm &b);

	std::string toString() const;
};

#endif // CONCERT_H
