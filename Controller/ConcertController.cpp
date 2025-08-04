#include "ConcertController.h"
#include "MainMenuView.h"
#include "musician_form.h"
#include "MusicalPiece.h"
#include "ScoreSelectView.h"
#include "Score.h"
#include "FileIO.h"
#include "HTML.h"
#include "MuttView.h"
#include "ConcertSummaryView.h"
#include "concert_info_form.h"
#include "rehearsal_form.h"
#include "pieceForm.h"
#include "VimView.h"
#include "logMessage.h"
#include <ncurses.h>
#include "SelectionView.h"
#include "AbookImporter.h"
#include "Utils.h"
#include <optional> // per std::optional
#include <cstdlib>  // per system()

void ConcertController::start() {
	clear();
	mvprintw(2, 2, "Load scores. Wait please...");
	refresh();
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

	std::vector<std::vector<MenuItem>> menuItems = { {
			{ "Exit", MenuCommand::Quit },
			{ "HTML", MenuCommand::HTML } },

	{ { "Edit Concert Info", MenuCommand::EditConcertInfo }, { "Delete Concert",
			MenuCommand::DeleteConcert } },

	{ { "Add Musician", MenuCommand::AddMusician }, { "Edit Musician",
			MenuCommand::EditMusician }, { "Delete Musician",
			MenuCommand::DeleteMusician } },

	{   // Piece
			{ "Add Piece", MenuCommand::AddPiece }, { "Edit / View Piece",
					MenuCommand::EditPiece }, { "Delete Piece",
					MenuCommand::DeletePiece } },

			{   // Rehearsal
			{ "Add Rehearsal", MenuCommand::AddRehearsal }, { "Edit Rehearsal",
					MenuCommand::EditRehearsal }, { "Delete Rehearsal",
					MenuCommand::DeleteRehearsal } },

			{ // Misc
			{ "Comment", MenuCommand::Comment }, { "Mail", MenuCommand::Mail },
					{ "TODO", MenuCommand::Todo } } };

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
		case MenuCommand::HTML: {
			generateHTML(concert);
			break;
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
		case MenuCommand::Mail:
			sendMail(concert);
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
			model.updateConcertInfo(concert, *existing); // source: concert, target: existing
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
	bool confirm = confirmDialog(stdscr);
	if (confirm)
		model.deleteConcert(concert);
}

void ConcertController::createMusician(Concert *concert) {
	clear();
	if (auto musiciansOpt = createEditMusician(nullptr)) {
		for (const auto &musician : *musiciansOpt) {
			model.addMusician(musician, *concert);
		}
	}
}

void ConcertController::editMusician(Concert *concert) {
	clear();
	SelectionView view;
	const std::vector<Musician> &musicians = concert->getMusicians();
	int choice = view.runChoiceForm(musicians);
	if (choice == -1)
		return;

	const Musician oldMusician = musicians.at(choice);
	std::optional<std::vector<Musician>> updated = createEditMusician(
			&oldMusician);

	if (updated && !updated->empty()) {
		// Aggiorna il musicista selezionato con il primo elemento
		model.updateMusician(oldMusician, updated->at(0), *concert);

		// Aggiungi eventuali altri musicisti nuovi
		for (size_t i = 1; i < updated->size(); ++i) {
			model.addMusician(updated->at(i), *concert);
		}
	}
}

void ConcertController::deleteMusician(Concert *concert) {
	clear();
	SelectionView view;
	const std::vector<Musician> &musicians = concert->getMusicians();
	int choice = view.runChoiceForm(musicians);
	if (choice == -1)
		return;
	bool confirm = confirmDialog(stdscr);
	if (confirm) {
		Musician m = concert->getMusicians().at(choice);
		model.deleteMusician(m, *concert);
	}
}

std::optional<std::vector<Musician>> ConcertController::createEditMusician(
		const Musician *musician) {
	std::vector<std::string> menuTitles = { "File" };
	std::vector<std::vector<MenuItem>> menuItems = { { { "New Musician",
			MenuCommand::AddMusician },
			{ "Save and Exit", MenuCommand::SaveExit }, { "Exit without saving",
					MenuCommand::Quit } } };
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
		std::vector<Musician> musicians;
		musicians = form.getMusicians();
		return musicians;
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
	std::optional<std::vector<MusicalPiece>> piecesOpt = createEditPiece(
			nullptr);
	if (piecesOpt) {
		for (const auto &piece : *piecesOpt) {
			model.addPiece(piece, *concert);
		}
	}
}

void ConcertController::editPiece(Concert *concert) {
	clear();
	SelectionView view;
	const std::vector<MusicalPiece> &pieces = concert->getProgram();
	int choice = view.runChoiceForm(pieces);
	if (choice == -1)
		return;

	MusicalPiece oldPiece = pieces.at(choice);
	std::optional<std::vector<MusicalPiece>> updatedPiecesOpt = createEditPiece(
			&oldPiece);
	if (updatedPiecesOpt) {
		// Aggiorna il pezzo modificato e aggiungi gli eventuali nuovi pezzi
		if (!updatedPiecesOpt->empty()) {
			model.updatePiece(oldPiece, (*updatedPiecesOpt)[0], *concert); // Primo elemento è il pezzo modificato
			for (size_t i = 1; i < updatedPiecesOpt->size(); ++i) {
				model.addPiece((*updatedPiecesOpt)[i], *concert);
			}
		}
	}
}

void ConcertController::deletePiece(Concert *concert) {
	clear();
	SelectionView view;
	const std::vector<MusicalPiece> &pieces = concert->getProgram(); // Presuppone getPieces() in Concert
	int choice = view.runChoiceForm(pieces);
	if (choice == -1)
		return;

	bool confirm = confirmDialog(stdscr);
	if (confirm) {
		MusicalPiece p = pieces.at(choice);
		model.deletePiece(p, *concert);
	}
}

std::optional<std::vector<MusicalPiece>> ConcertController::createEditPiece(
		MusicalPiece *piece) {
	std::vector<std::string> menuTitles = { "File", "Score" };
	std::vector<std::vector<MenuItem>> menuItems = { { { "New Piece",
			MenuCommand::AddPiece },
			{ "Youtube", MenuCommand::Youtube },
			{ "Save and Exit", MenuCommand::SaveExit },
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
		clear();
		form.show();
		MenuCommand command = form.getCommand();

		switch (command) {
		case MenuCommand::SaveExit: {
			std::vector<MusicalPiece> pieces;
			pieces = form.getPieces();
			return pieces;
		}
		case MenuCommand::Quit:
			return std::nullopt;
		case MenuCommand::Youtube:
			openInBrowser(piece);
			break;
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
	clear();
	if (auto rehearsalsOpt = createEditRehearsal(nullptr)) {
		for (const auto &rehearsal : *rehearsalsOpt) {
			model.addRehearsal(rehearsal, *concert);
		}
	}
}

void ConcertController::editRehearsal(Concert *concert) {
	clear();
	SelectionView view;
	const std::vector<Rehearsal> &rehearsals = concert->getRehearsals();
	int choice = view.runChoiceForm(rehearsals);
	if (choice == -1)
		return;

	const Rehearsal oldRehearsal = rehearsals.at(choice);
	std::optional<std::vector<Rehearsal>> updated = createEditRehearsal(
			&oldRehearsal);

	if (updated && !updated->empty()) {
		// Aggiorna la prova selezionata con il primo elemento
		model.updateRehearsal(oldRehearsal, updated->at(0), *concert);

		// Aggiunge eventuali nuove prove
		for (size_t i = 1; i < updated->size(); ++i) {
			model.addRehearsal(updated->at(i), *concert);
		}
	}
}

void ConcertController::deleteRehearsal(Concert *concert) {
	clear();
	SelectionView view;
	const std::vector<Rehearsal> &rehearsals = concert->getRehearsals();
	int choice = view.runChoiceForm(rehearsals);
	if (choice == -1)
		return;

	bool confirm = confirmDialog(stdscr);
	if (confirm) {
		Rehearsal r = rehearsals.at(choice);
		model.deleteRehearsal(r, *concert);
	}
}

std::optional<std::vector<Rehearsal>> ConcertController::createEditRehearsal(
		const Rehearsal *rehearsal) {
	std::vector<std::string> menuTitles = { "File" };
	std::vector<std::vector<MenuItem>> menuItems = { { { "New Rehearsal",
			MenuCommand::AddRehearsal }, { "Save and Exit",
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
		std::vector<Rehearsal> rehearsals = form.getRehearsals();
		return rehearsals;
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

void ConcertController::openInBrowser(const MusicalPiece *piece) {
	std::string url = piece->getYoutubeLink();
	if (url.empty() || url.find("http") != 0) {
	    LOG_MSG("Invalid or missing YouTube URL");
	    return;
	}
#ifdef _WIN32
	std::string command = "start \"\" \"" + url + "\"";
#elif __APPLE__
	std::string command = "open \"" + url + "\"";
#else // Linux, Unix-like
	std::string command = "xdg-open \"" + url + "\"";
#endif
	system(command.c_str());
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

	bool confirm = confirmDialog(stdscr);
	if (confirm) {
		Score s = scores.at(choice);
		model.deleteScore(s, *piece);
	}
	return confirm;
}

bool ConcertController::addScore(MusicalPiece *piece) {
	clear();
	std::vector<std::string> scorePaths = model.getScorePaths();
	ScoreSelectView scoreSelectView;
	std::optional<std::vector<std::string>> paths = scoreSelectView.show(scorePaths);

	if (!paths || paths->empty())
	    return false;

	for (const auto &path : *paths) {
	        Score s;
	        s.setPath(path);
	        model.addScore(s, *piece);
	    }

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

void ConcertController::sendMail(Concert *concert) {
	// memorizza tutti i musicisti che hanno una mail
	std::vector<Musician> musicians = concert->getMusicians();
	std::vector<Musician> musicianWithMail;
	for (const Musician &m : musicians) {
		if (!empty(m.getMail()))
			musicianWithMail.push_back(m);
	}

	clear();
	SelectionView selectionView;
	int choice = selectionView.runChoiceForm(musicianWithMail);
	if (choice == -1)
		return;
	std::string mail = musicianWithMail.at(choice).getMail();

	MuttView muttView;
	muttView.launchMutt(mail);
}

void ConcertController::autofillFromAbook(const std::string &name,
		MusicianForm &form) {
	auto maybeM = AbookImporter::lookupByName(name);
	if (maybeM.has_value()) {
		const Musician &m = maybeM.value();
		form.setAutoFilledFields(m);
	}
}

void ConcertController::sort() {
	model.sortConcerts();
}

void ConcertController::save() {
	model.saveToFile(FileIO::savePath + "/concerts.xml");
}

void ConcertController::load() {
	model.loadScorePaths();
	model.loadFromFile(FileIO::savePath + "/concerts.xml");
}

void ConcertController::generateHTML(Concert* c) {
	HTML html;
	html.saveHTML(*c);

}
