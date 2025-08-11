#pragma once

#include <vector>
#include <string>
#include "Concert.h"
#include <vector>

class Model {
public:
    // Costruttori
    Model() = default;

    // Caricamento e salvataggio
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename) const;

    // Accesso ai concerti
    const std::vector<Concert>& getConcerts() const;
    std::vector<Concert>& getConcerts(); // versione modificabile

    Concert* getConcert(int index);

    // Operazioni sui concerti
    void addConcert(const Concert& concert);
    void updateConcertInfo(const Concert &source, Concert &target);
    void updateMusician(const Musician& oldM, const Musician& newM, Concert& concert);
    bool movePiece(int pos, int offset, std::vector<MusicalPiece> &program);
    void addPiece(const MusicalPiece& piece, Concert& concert);
    void updatePiece(const MusicalPiece& oldPiece, const MusicalPiece& newPiece, Concert& concert);
    void deletePiece(const MusicalPiece& piece, Concert& concert);
    Rehearsal* createEmptyRehearsal(Concert &concert);
    Musician* createEmptyMusician(Concert &concert);
    void removeRehearsal(Rehearsal* rehearsal, Concert &concert);
    void removeMusician(Musician* musician, Concert &concert);
    void updateRehearsal(const Rehearsal& oldRehearsal, const Rehearsal& newRehearsal, Concert& concert);
    void deleteConcert(size_t index);
    void deleteConcert(Concert* c);
    void addScore(const Score &score, MusicalPiece &piece);
    void deleteScore(const Score &score, MusicalPiece &piece);
    void addComment(const std::string& comment, Concert* concert);
    void clear(); // rimuove tutto
	void sortConcerts();
	void loadScorePaths();
	const std::vector<std::string>& getScorePaths() const;
    size_t concertCount() const;
    static void calculateMusicianSalary(int rehearsal, int concerts, bool soloist,
    		double* baseSalary, double* vacationCompensation, double* insurances, double* salary);
private:
    std::vector<Concert> concerts;
    std::vector<std::string> scorePaths;


};
