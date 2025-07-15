// ConcertController.h
#pragma once
#include "Concert.h"
#include <optional>
class ConcertController {
public:
void loadXML(std::vector<Concert>& concerts);

void saveXML(std::vector<Concert>& concerts);
// Funzione di confronto per Concert basata sulla prima data nel vettore dates

void sortConcerts(std::vector<Concert> &concerts);
    std::optional<Concert> createConcert();
void listConcerts(std::vector<Concert>& concerts);
void editConcert(std::vector<Concert>& concerts);
void deleteConcert(std::vector<Concert>& concerts);
std::optional<Concert> editConcertSingle(const Concert& existing);

private:
// ritorna -1 se nessun concerto è selezionato, altrimenti il numero del concerto
int selectConcert(std::vector<Concert>& concerts);
};

