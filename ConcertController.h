// ConcertController.h
#pragma once
#include "Concert.h"

class ConcertController {
public:
    Concert createConcert();
void listConcerts(std::vector<Concert>& concerts);
void editConcert(std::vector<Concert>& concerts);
void deleteConcert(std::vector<Concert>& concerts);
Concert editConcertSingle(const Concert& existing);

private:
// ritorna -1 se nessun concerto è selezionato, altrimenti il numero del concerto
int selectConcert(std::vector<Concert>& concerts);
};

