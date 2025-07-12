#pragma once

#include <string>
#include <vector>
#include "Musician.h"
#include "MusicalPiece.h"
#include "Rehearsal.h"
#include <ctime>

class Concert
{
public:
  Concert(); // costruttore di default
  Concert(const std::string &title);

  void setTitle(const std::string &t);
  const std::string &getTitle() const;

  void setPlaces(const std::vector<std::string> &p);
  const std::vector<std::string> &getPlaces() const;

  void addDate(std::string &d);
  void setDatesAsString(const std::vector<std::string> &d);
  std::vector<std::string> getDatesAsString() const;
  const std::vector<tm> &getDatesAsTm() const;

  void setProgram(const std::vector<MusicalPiece> &p);
  void setMusicians(const std::vector<Musician> &m);
  void addMusician(const Musician &m);

  std::vector<MusicalPiece> getProgram() const;
  std::vector<Musician> getMusicians() const;

  void addRehearsal(const Rehearsal &r);
  const std::vector<Rehearsal> &getRehearsals() const;
  void setRehearsals(const std::vector<Rehearsal> &r);

  void setToDo(const std::string &todo);
  const std::string getToDo() const;

  bool hasMusicians() const;

  int getDuration() const;
  void setComment(const std::string &c) { comment = c; }
  std::string getComment() const { return comment; }

private:
  std::string title;
  std::string todo;
  std::vector<std::string> places;
  std::vector<tm> dates;
  std::vector<MusicalPiece> program;
  std::vector<Musician> musicians;
  std::vector<Rehearsal> rehearsals;
  std::string comment;
  static std::string tmToString(const std::tm &date);
  // Converte stringa "gg.mm.aaaa" in tm
  // Ritorna true se la conversione è andata a buon fine, false altrimenti
  static bool stringToTm(const std::string &dateStr, std::tm &date);
  // Funzione di confronto per tm
  static bool compareTm(const std::tm &a, const std::tm &b);
};
