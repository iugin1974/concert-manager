#include "Model.h"
#include "logMessage.h"
#include <string>
#include <algorithm> // std::sort
#include <stdexcept> // per std::out_of_range
#include "FileIO.h"

bool compareConcertByFirstDate(const Concert &a, const Concert &b)
{
  const auto &datesA = a.getDatesAsTm();
  const auto &datesB = b.getDatesAsTm();

  if (datesA.empty() && datesB.empty())
    return false; // uguali, non cambia l'ordine
  if (datesA.empty())
    return false; // a senza date va dopo b
  if (datesB.empty())
    return true; // b senza date va dopo a

  // Confronta la prima data usando lo stesso criterio di prima
  const std::tm &dA = datesA.front();
  const std::tm &dB = datesB.front();

  if (dA.tm_year != dB.tm_year)
    return dA.tm_year < dB.tm_year;
  if (dA.tm_mon != dB.tm_mon)
    return dA.tm_mon < dB.tm_mon;
  return dA.tm_mday < dB.tm_mday;
}

void Model::sortConcerts()
{
  std::sort(concerts.begin(), concerts.end(), compareConcertByFirstDate);
}

// Caricamento da file
bool Model::loadFromFile(const std::string &filename) {
	FileIO f;
	concerts = f.loadConcertsFromXML(filename);
	return true; // TODO
}

// Salvataggio su file
bool Model::saveToFile(const std::string &filename) const {
	FileIO f;
	f.saveConcertsToXML(concerts, filename);
	return true; // TODO
}

// Accesso ai concerti
const std::vector<Concert>& Model::getConcerts() const {
	return concerts;
}

std::vector<Concert>& Model::getConcerts() {
	return concerts;
}

	Concert* Model::getConcert(int index) {
	    if (index < 0 || static_cast<size_t>(index) >= concerts.size()) {
	        throw std::out_of_range("Index out of range for vector 'concerts'");
	    }

	    return &concerts[index];
	}

// Operazioni
void Model::addConcert(const Concert &concert) {
	concerts.push_back(concert);
}

void Model::updateConcertInfo(const Concert &source, Concert &target) {
    target.setTitle(source.getTitle());
    target.setPlaces(source.getPlaces());
    target.setDatesAsString(source.getDatesAsString());
}

void Model::addMusician(const Musician &musician, Concert &concert) {
	std::vector<Musician> &musicians = concert.getMusicians();
	musicians.push_back(musician);
}

void Model::updateMusician(const Musician &oldM, const Musician &newM,
		Concert &concert) {
	std::vector<Musician> &musicians = concert.getMusicians();
	for (Musician &m : musicians) {
		if (m.isSameAs(oldM)) { // usa un confronto adeguato
			m = newM;
			return;
		}
	}
}

void Model::deleteMusician(Musician &musician, Concert &concert) {
	std::vector<Musician> &musicians = concert.getMusicians();
	for (auto it = musicians.begin(); it != musicians.end(); ++it) {
		if (it->isSameAs(musician)) {
			musicians.erase(it);
			return;
		}
	}
}

void Model::addPiece(const MusicalPiece &piece, Concert &concert) {
	std::vector<MusicalPiece> &pieces = concert.getProgram();
	pieces.push_back(piece);
}

void Model::updatePiece(const MusicalPiece &oldPiece,
		const MusicalPiece &newPiece, Concert &concert) {
	std::vector<MusicalPiece> &pieces = concert.getProgram();
	for (MusicalPiece &p : pieces) {
		if (p.getTitle() == oldPiece.getTitle()
				&& p.getComposer() == oldPiece.getComposer()) {
			p = newPiece;
			return;
		}
	}
}

void Model::deletePiece(const MusicalPiece &piece, Concert &concert) {
	std::vector<MusicalPiece> &pieces = concert.getProgram();
	for (auto it = pieces.begin(); it != pieces.end(); ++it) {
		if (it->getTitle() == piece.getTitle()
				&& it->getComposer() == piece.getComposer()) {
			pieces.erase(it);
			break;
		}
	}
}

void Model::deleteConcert(Concert* concert) {
	for (auto it = concerts.begin(); it != concerts.end(); it++) {
		if (it->isSameAs(*concert)) {
			concerts.erase(it);
			return;
		}
	}
}

void Model::deleteConcert(size_t index) {
	if (index < concerts.size()) {
		concerts.erase(concerts.begin() + index);
	}
}

void Model::addRehearsal(const Rehearsal &rehearsal, Concert &concert) {
	std::vector<Rehearsal> &rehearsals = concert.getRehearsals();
	rehearsals.push_back(rehearsal);
}

void Model::updateRehearsal(const Rehearsal &oldRehearsal,
		const Rehearsal &newRehearsal, Concert &concert) {
	std::vector<Rehearsal> &rehearsals = concert.getRehearsals();
	for (Rehearsal &r : rehearsals) {
		if (r.isSameAs(oldRehearsal)) { // Presuppone un metodo isSameAs() in Rehearsal
			r = newRehearsal;
			return;
		}
	}
}

void Model::deleteRehearsal(const Rehearsal &rehearsal, Concert &concert) {
	std::vector<Rehearsal> &rehearsals = concert.getRehearsals();
	for (auto it = rehearsals.begin(); it != rehearsals.end(); ++it) {
		if (it->isSameAs(rehearsal)) {
			rehearsals.erase(it);
			return;
		}
	}
}

// aggiungi una score al concert
void Model::addScore(const Score &score, MusicalPiece &piece) {
    std::vector<Score> &scores = piece.getScores();
    scores.push_back(score);
}

// elimina una score dal concert
void Model::deleteScore(const Score &score, MusicalPiece &piece) {
    std::vector<Score> &scores = piece.getScores();
    for (auto it = scores.begin(); it != scores.end(); ++it) {
        if (it->getPath() == score.getPath()) {
            scores.erase(it);
            return;
        }
    }
}

void Model::addComment(const std::string& comment, Concert* concert) {
	concert->setComment(comment);
}

void Model::clear() {
	concerts.clear();
}

size_t Model::concertCount() const {
	return concerts.size();
}
