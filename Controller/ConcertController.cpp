#include "ConcertController.h"
#include "MainMenuView.h"
#include "musician_form.h"
#include "MusicalPiece.h"
#include "ScoreSelectView.h"
#include "Score.h"
#include "FileIO.h"
#include "ConcertSummaryView.h"
#include "concert_info_form.h"
#include "rehearsal_form.h"
#include "pieceForm.h"
#include "VimView.h"
#include "logMessage.h"
#include <ncurses.h>
#include "SelectionView.h"
#include <optional> // per std::optional
#include <cstdlib>  // per system()

void ConcertController::start() {
	load();
	while (true) {
		MainMenuView mainMenuView;
		MainMenuView::Action action = mainMenuView.show();

		switch (action) {
		case MainMenuView::ADD_CONCERT: {
			bool created = createEditConcert(nullptr);
			if (created) {
				sort();
				save();
			}
			break;
		}
		case MainMenuView::MANAGE_CONCERTS:
			manageConcerts();
			break;
		case MainMenuView::SAVE:
			save();
			break;
		case MainMenuView::EXIT:
			save();
			endwin();
			return;  // esce dal cir  e dalla funzione
		}
		// Qui la finestra viene chiusa e ricreata a ogni ciclo (dipende da come show() funziona)
		// Se non lo fa, potresti aver bisogno di un metodo apposito per pulire e ridisegnare
	}
}

void ConcertController::manageConcerts() {
	//sceglie il concerto
	clear();
	SelectionView view;
	std::vector<Concert> &concerts = model.getConcerts();
	int choice = view.runChoiceForm(concerts);
	if (choice == -1)
		return;
	Concert *concert;
	concert = model.getConcert(choice);
// Mostra la finestra con le possibilità di modificare il concerto
	std::vector<std::string> menuTitles = { "File", "Concert", "Musician",
			"Piece", "Rehearsal", "Tools" };

	std::vector<std::vector<MenuItem>> menuItems =
			{ { { "Exit", MenuCommand::Quit } },

			{ { "Edit Concert Info", MenuCommand::EditConcertInfo }, {
					"Delete Concert", MenuCommand::DeleteConcert } },

			{ { "Add Musician", MenuCommand::AddMusician }, { "Edit Musician",
					MenuCommand::EditMusician }, { "Delete Musician",
					MenuCommand::DeleteMusician } },

			{   // Piece
					{ "Add Piece", MenuCommand::AddPiece }, {
							"Edit / View Piece", MenuCommand::EditPiece }, {
							"Delete Piece", MenuCommand::DeletePiece } },

					{   // Rehearsal
					{ "Add Rehearsal", MenuCommand::AddRehearsal }, {
							"Edit Rehearsal", MenuCommand::EditRehearsal }, {
							"Delete Rehearsal", MenuCommand::DeleteRehearsal } },

					{ // Misc
					{ "Comment", MenuCommand::Comment }, { "TODO",
							MenuCommand::Todo } } };

	clear();
	MenuBar menuBar(stdscr);
	menuBar.setTitles(menuTitles);
	menuBar.setItems(menuItems);

	ConcertSummaryView form;
	form.setMenuBar(menuBar);

	while (true) {
		clear();
		form.setConcert(*concert);
		form.show();
		MenuCommand command = form.getCommand();
		switch (command) {
		case MenuCommand::Quit: {
			return;
		}
		case MenuCommand::EditConcertInfo: {
			if (createEditConcert(concert)) {
				sort();
				save();
			}
			break;
		}
		case MenuCommand::DeleteConcert:
			deleteConcert(concert);
			save();
			return;
		case MenuCommand::AddMusician:
			createMusician(concert);
			save();
			break;
		case MenuCommand::EditMusician:
			editMusician(concert);
			save();
			break;
		case MenuCommand::DeleteMusician:
			deleteMusician(concert);
			save();
			break;
		case MenuCommand::AddPiece:
			createPiece(concert);
			save();
			break;
		case MenuCommand::EditPiece:
			editPiece(concert);
			save();
			form.show();
			break;
		case MenuCommand::DeletePiece:
			deletePiece(concert);
			save();
			break;
		case MenuCommand::AddRehearsal:
			createRehearsal(concert);
			save();
			break;
		case MenuCommand::EditRehearsal:
			editRehearsal(concert);
			save();
			break;
		case MenuCommand::DeleteRehearsal:
			deleteRehearsal(concert);
			save();
			break;
		case MenuCommand::Comment:
			if (commentConcert(concert)) {
				save();
			}
			break;
		case MenuCommand::Todo:
			break;

		default:
			break;
		}
	}
}

bool ConcertController::createEditConcert(Concert *existing) {
	std::vector<std::string> menuTitles = { "File" };
	std::vector<std::vector<MenuItem>> menuItems = { { { "Save and Exit",
			MenuCommand::SaveExit },
			{ "Exit without saving", MenuCommand::Quit } } };

	MenuBar menuBar(stdscr);
	menuBar.setTitles(menuTitles);
	menuBar.setItems(menuItems);

	ConcertInfoForm form;
	form.setMenuBar(menuBar);
	form.setConcert(existing);
	form.show();
	MenuCommand command = form.getCommand();
	switch (command) {
	case MenuCommand::SaveExit: {
		Concert concert; // dati aggiornati dall'utente
		concert.setTitle(form.getTitle());
		concert.setDatesAsString(form.getDatesAsVector());
		concert.setPlaces(form.getPlacesAsVector());

		if (existing) {
		    model.updateConcertInfo(concert, *existing);  // source: concert, target: existing
		} else {
		    model.addConcert(concert); // nuovo concerto
		}
		return true;
	}
	case MenuCommand::Quit: {

		return false;
	}
	default: {

	}
	}
	return false;
}

void ConcertController::deleteConcert(Concert *concert) {
	model.deleteConcert(concert);
}

void ConcertController::createMusician(Concert *concert) {
	clear();
	std::optional<Musician> m = createEditMusician(nullptr);
	if (m)
		model.addMusician(*m, *concert);
}

void ConcertController::editMusician(Concert *concert) {
	clear();
	SelectionView view;
	const std::vector<Musician> &musicians = concert->getMusicians();
	int choice = view.runChoiceForm(musicians);
	if (choice == -1)
		return;

	const Musician oldMusician = musicians.at(choice);
	std::optional<Musician> updated = createEditMusician(&oldMusician);
	if (updated) {
		model.updateMusician(oldMusician, *updated, *concert);
	}
}

void ConcertController::deleteMusician(Concert *concert) {
	clear();
	SelectionView view;
	const std::vector<Musician> &musicians = concert->getMusicians();
	int choice = view.runChoiceForm(musicians);
	if (choice == -1)
		return;

	Musician m = concert->getMusicians().at(choice);
	model.deleteMusician(m, *concert);
}

std::optional<Musician> ConcertController::createEditMusician(
		const Musician *musician) {
	std::vector<std::string> menuTitles = { "File" };
	std::vector<std::vector<MenuItem>> menuItems = { { { "Save and Exit",
			MenuCommand::SaveExit },
			{ "Exit without saving", MenuCommand::Quit } } };
	MenuBar menuBar(stdscr);
	menuBar.setTitles(menuTitles);
	menuBar.setItems(menuItems);

	MusicianForm form;
	form.setMusician(musician);
	form.setMenuBar(menuBar);
	form.show();
	MenuCommand command = form.getCommand();
	switch (command) {
	case MenuCommand::SaveExit: {
		Musician m;
		m.setName(form.getName());
		m.setPhone(form.getPhone());
		m.setInstrument(form.getInstrument());
		m.setMail(form.getMail());
		m.setAddress(form.getAddress());
		m.setGage(form.getGage());
		m.setSoloist(form.getSoloist());
		m.setRehearsalNumber(form.getRehearsalNumber());
		m.setConcertNumber(form.getConcertNumber());
		m.setTravelCosts(form.getTravelCosts());
		return m;
	}
	case MenuCommand::Quit: {
		return std::nullopt;
	}
	default: {
		break;
	}
	}
	return std::nullopt;
}

void ConcertController::createPiece(Concert *concert) {
	clear();
	std::optional<MusicalPiece> piece = createEditPiece(nullptr);
	if (piece)
		model.addPiece(*piece, *concert);
}

void ConcertController::editPiece(Concert *concert) {
	clear();
	SelectionView view;
	const std::vector<MusicalPiece> &pieces = concert->getProgram();
	int choice = view.runChoiceForm(pieces);
	if (choice == -1)
		return;

	MusicalPiece oldPiece = pieces.at(choice);
	std::optional<MusicalPiece> updated = createEditPiece(&oldPiece);
	if (updated) {
		model.updatePiece(oldPiece, *updated, *concert);
	}
}

void ConcertController::deletePiece(Concert *concert) {
	clear();
	SelectionView view;
	const std::vector<MusicalPiece> &pieces = concert->getProgram(); // Presuppone getPieces() in Concert
	int choice = view.runChoiceForm(pieces);
	if (choice == -1)
		return;

	MusicalPiece p = pieces.at(choice);
	model.deletePiece(p, *concert);
}

std::optional<MusicalPiece> ConcertController::createEditPiece(
		MusicalPiece *piece) {
	std::vector<std::string> menuTitles = { "File", "Score" };
	std::vector<std::vector<MenuItem>> menuItems = { { { "Save and Exit",
			MenuCommand::SaveExit },
			{ "Exit without saving", MenuCommand::Quit } }, {   // Scores
			{ "Add Score", MenuCommand::AddScore }, { "Delete Score",
					MenuCommand::DeleteScore }, { "View Score",
					MenuCommand::ViewScore } } };

	MenuBar menuBar(stdscr);
	menuBar.setTitles(menuTitles);
	menuBar.setItems(menuItems);

	PieceForm form;  // Devi avere questa form
	form.setPiece(piece);
	form.setMenuBar(menuBar);

	while (true) {
		form.show();
		MenuCommand command = form.getCommand();

		switch (command) {
		case MenuCommand::SaveExit: {
			MusicalPiece p;
			p.setComposer(form.getComposer());
			p.setTitle(form.getTitle());
			p.setDuration(form.getDuration());
			p.setChoir(form.hasChoir());
			p.setSingerPart(form.getSinger());
			p.setInstruments(form.getInstruments());
			if (piece)
				p.setScores(piece->getScores()); // copia scores aggiornate

			return p;
		}
		case MenuCommand::Quit:

			return std::nullopt;
		case MenuCommand::AddScore:
			addScore(piece);
			break;
		case MenuCommand::DeleteScore:
			deleteScore(piece);
			break;
		case MenuCommand::ViewScore:
			viewScore(piece);
			break;
		default:
			break;
		}
	}
	return std::nullopt;
}

void ConcertController::createRehearsal(Concert *concert) {
	std::optional<Rehearsal> r = createEditRehearsal(nullptr);
	if (r)
		model.addRehearsal(*r, *concert);
}

void ConcertController::editRehearsal(Concert *concert) {
	clear();
	SelectionView view;
	const std::vector<Rehearsal> &rehearsals = concert->getRehearsals();
	int choice = view.runChoiceForm(rehearsals);
	if (choice == -1)
		return;

	const Rehearsal oldRehearsal = rehearsals.at(choice);
	std::optional<Rehearsal> updated = createEditRehearsal(&oldRehearsal);
	if (updated) {
		model.updateRehearsal(oldRehearsal, *updated, *concert);
	}
}

void ConcertController::deleteRehearsal(Concert *concert) {
	clear();
	SelectionView view;
	const std::vector<Rehearsal> &rehearsals = concert->getRehearsals();
	int choice = view.runChoiceForm(rehearsals);
	if (choice == -1)
		return;

	Rehearsal r = rehearsals.at(choice);
	model.deleteRehearsal(r, *concert);
}

std::optional<Rehearsal> ConcertController::createEditRehearsal(
		const Rehearsal *rehearsal) {
	std::vector<std::string> menuTitles = { "File" };
	std::vector<std::vector<MenuItem>> menuItems = { { { "Save and Exit",
			MenuCommand::SaveExit },
			{ "Exit without saving", MenuCommand::Quit } } };

	MenuBar menuBar(stdscr);
	menuBar.setTitles(menuTitles);
	menuBar.setItems(menuItems);

	RehearsalForm form;  // Presuppone l'esistenza di questa form
	form.setRehearsal(rehearsal);
	form.setMenuBar(menuBar);
	form.show();
	MenuCommand command = form.getCommand();

	switch (command) {
	case MenuCommand::SaveExit: {
		Rehearsal r;
		r.setDate(form.getDate());
		r.setStartTime(form.getStartTime());
		r.setPlace(form.getPlace());
		r.setMusicians(form.getMusicians());

		return r;
	}
	case MenuCommand::Quit:
		return std::nullopt;
	default:
		break;
	}

	return std::nullopt;
}

void ConcertController::viewScore(const MusicalPiece *piece) {
	clear();
	SelectionView view;
	const std::vector<Score> &scores = piece->getScores();
	int choice = view.runChoiceForm(scores);
	if (choice == -1)
		return;
	std::string path = Score::basePathScores + "/"
			+ piece->getScores().at(choice).getPath();
#ifdef _WIN32
    // Windows
    std::string cmd = "start \"\" \"" + path + "\"";
    system(cmd.c_str());
#elif __APPLE__
    // macOS
    std::string cmd = "open \"" + path + "\"";
    system(cmd.c_str());
#else
	// Linux/Unix
	std::string cmd = "xdg-open \"" + path + "\"";
	system(cmd.c_str());
#endif
}

bool ConcertController::deleteScore(MusicalPiece *piece) {
	clear();
	SelectionView view;
	const std::vector<Score> &scores = piece->getScores();
	int choice = view.runChoiceForm(scores);
	if (choice == -1)
		return false;

	Score s = scores.at(choice);
	model.deleteScore(s, *piece);
	return true;
}

bool ConcertController::addScore(MusicalPiece *piece) {
	clear();
	ScoreSelectView scoreSelectView;
	std::optional<std::string> path = scoreSelectView.show();
	if (path == std::nullopt)
		return false;
	Score s;
	s.setPath(path.value());
	model.addScore(s, *piece);
	return true;
}

bool ConcertController::commentConcert(Concert *concert) {
	std::string comment = concert->getComment();
	VimView vimv;
	bool commented = vimv.edit(comment);
	if (commented) {
		comment = vimv.getText();
		model.addComment(comment, concert);
	}
	return commented;
}

void ConcertController::sort() {
	model.sortConcerts();
}

void ConcertController::save() {
	model.saveToFile(FileIO::savePath + "/concerts.xml");
}

void ConcertController::load() {
	model.loadFromFile(FileIO::savePath + "/concerts.xml");
}
