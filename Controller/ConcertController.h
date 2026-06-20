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
#include "Utils.h"
#include "Concert.h"
#include "Model.h"
#include "MoveElementView.h"
#include "SelectionView.h"
#include <algorithm>
#include "MenuBar.h"

class MusicianForm;

class ConcertController {
	public:
		void start();
		void autofillFromAbook(const std::string& name, MusicianForm& form);
	private:
		bool createEditConcert(Concert *concert);
		void manageConcerts();
		template <typename T>
			void deleteElement(std::vector<T>& elements) {
				clear();
				SelectionView view;
				int choice = view.runChoiceForm(elements);
				if (choice == -1) return;
				bool confirm = confirmDialog(stdscr);
				if (confirm) {
					T& element = elements.at(choice);
					elements.erase(std::remove_if(elements.begin(), elements.end(),
								[&](const T& t) { return &t == &element; }),
							elements.end());
				}
			}

		void deleteConcert(Concert* concert);
		void createMusician(Concert* concert);
		void editMusician(Concert* concert);
		std::optional<std::vector<Musician>> createEditMusician(const Musician *musicians);
		void createPiece(Concert* concert);
		void editPiece(Concert* concert);
		std::optional<std::vector<MusicalPiece>> createEditPiece(MusicalPiece *MusicalPieces);

		template <typename T>
			void moveElement(std::vector<T>& elements) {
				MoveElementView view;
				view.show(elements);
			};

		void createRehearsal(Concert* concert);
		void editRehearsal(Concert* concert);

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
