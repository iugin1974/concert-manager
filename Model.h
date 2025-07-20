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
    void updateConcertInfo(const Concert& updated, Concert& concert);
    void addMusician(const Musician& musician, Concert& concert);
    void updateMusician(const Musician& oldM, const Musician& newM, Concert& concert);
    void deleteMusician(Musician& musician, Concert& concert);
    void addPiece(const MusicalPiece& piece, Concert& concert);
    void updatePiece(const MusicalPiece& oldPiece, const MusicalPiece& newPiece, Concert& concert);
    void deletePiece(const MusicalPiece& piece, Concert& concert);
    void addRehearsal(const Rehearsal& rehearsal, Concert& concert);
    void updateRehearsal(const Rehearsal& oldRehearsal, const Rehearsal& newRehearsal, Concert& concert);
    void deleteRehearsal(const Rehearsal& rehearsal, Concert& concert);
    void deleteConcert(size_t index);
    void deleteConcert(Concert* c);
    void clear(); // rimuove tutto
	void sortConcerts();
    size_t concertCount() const;

private:
    std::vector<Concert> concerts;


};
