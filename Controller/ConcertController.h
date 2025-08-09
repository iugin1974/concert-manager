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

class MusicianForm;

class ConcertController {
public:
	void start();
	void autofillFromAbook(const std::string& name, MusicianForm& form);
	bool movePiece(Concert& concert, int pos, int offset);

private:
	bool createEditConcert(Concert *concert);
	void manageConcerts();

	void deleteConcert(Concert* concert);
	void createMusician(Concert* concert);
	void editMusician(Concert* concert);
	std::optional<std::vector<Musician>> createEditMusician(const Musician *musicians);
	void deleteMusician(Concert* concert);

	void createPiece(Concert* concert);
	void editPiece(Concert* concert);
	std::optional<std::vector<MusicalPiece>> createEditPiece(MusicalPiece *MusicalPieces);
	void deletePiece(Concert* concert);
	void movePiece(Concert* concert);

	void createRehearsal(Concert* concert);
	void editRehearsal(Concert* concert);
	void deleteRehearsal(Concert* concert);
	std::optional<std::vector<Rehearsal>> createEditRehearsal(const Rehearsal* rehearsal);

	bool addScore(MusicalPiece* piece);
	bool deleteScore(MusicalPiece* piece);
	void viewScore(const MusicalPiece* piece);
	void openInBrowser(const MusicalPiece *piece);
	void generateHTML(Concert* c);
	void generateCSV(Concert* c);
	bool commentConcert(Concert* concert);
	void sendMail(Concert *concert);

	void save();
	void load();
	void sort();

	Model model;
};



#endif /* CONCERTCONTROLLER_H_ */
