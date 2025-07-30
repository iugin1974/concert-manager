/*
 * ConcertController.h
 *
 *  Created on: 17.07.2025
 *      Author: eugenio
 */

#ifndef CONCERTCONTROLLER_H_
#define CONCERTCONTROLLER_H_

#include <vector>
#include <optional>
#include "Concert.h"
#include "Model.h"

class ConcertController {
public:
	void start();
private:
	bool createEditConcert(Concert *concert);
	void manageConcerts();

	void deleteConcert(Concert* concert);
	void createMusician(Concert* concert);
	void editMusician(Concert* concert);
	std::optional<Musician> createEditMusician(const Musician *musicians);
	void deleteMusician(Concert* concert);

	void createPiece(Concert* concert);
	void editPiece(Concert* concert);
	std::optional<MusicalPiece> createEditPiece(MusicalPiece *MusicalPieces);
	void deletePiece(Concert* concert);

	void createRehearsal(Concert* concert);
	void editRehearsal(Concert* concert);
	void deleteRehearsal(Concert* concert);
	std::optional<Rehearsal> createEditRehearsal(const Rehearsal* rehearsal);

	bool addScore(MusicalPiece* piece);
	bool deleteScore(MusicalPiece* piece);
	void viewScore(const MusicalPiece* piece);

	bool commentConcert(Concert* concert);

	void save();
	void load();
	void sort();

	Model model;
};



#endif /* CONCERTCONTROLLER_H_ */
