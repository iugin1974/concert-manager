#include <form.h>
#include <ncurses.h>
#include <string>
#include "pieceForm.h"
#include "Utils.h"

PieceForm::~PieceForm() {
	closeForm();
}

void PieceForm::setMenuBar(const MenuBar &bar) {
	menuBar = bar;
}

void PieceForm::setPiece(const MusicalPiece *piece) {
	existing = piece;
}

void PieceForm::init_form() {
	if (form == nullptr) {
	// 1. Crea campi
	int row = 2;
	fields[0] = new_field(1, 40, row++, 20, 0, 0);  // Composer
	fields[1] = new_field(1, 40, row++, 20, 0, 0);  // Title
	fields[2] = new_field(1, 10, row++, 20, 0, 0);  // Duration (MM:SS)
	fields[3] = new_field(1, 4, row++, 20, 0, 0);  // Choir (Yes/No)
	set_field_buffer(fields[3], 0, "[ ]");
	field_opts_off(fields[3], O_EDIT); // Non editabile manualmente
	fields[4] = new_field(1, 40, row++, 20, 0, 0); // Singer part
	fields[5] = new_field(1, 40, row++, 20, 0, 0); // Instruments
	fields[6] = nullptr;

	for (int i = 0; i < NUMBER_OF_FIELDS; ++i) {
		set_field_back(fields[i], A_UNDERLINE);
		field_opts_off(fields[i], O_AUTOSKIP);
	}
	}
	// 2. Pre-compila se già esistente
	if (existing) {
		set_field_buffer(fields[0], 0, existing->getComposer().c_str());
		set_field_buffer(fields[1], 0, existing->getTitle().c_str());
		set_field_buffer(fields[2], 0,
				convertToMMSS(existing->getDuration()).c_str());
		hasChoiristChecked = existing->hasChoir();
		set_field_buffer(fields[3], 0, hasChoiristChecked ? "[X]" : "[ ]");
		set_field_buffer(fields[4], 0, existing->getSingerPart().c_str());
		set_field_buffer(fields[5], 0, existing->getInstruments().c_str());
	}

	form = new_form(fields);
}

void PieceForm::show() {
	init_form();
	post_form(form);

	int row = 2;
	mvprintw(row++, 2, "Composer:");
	mvprintw(row++, 2, "Title*:");
	mvprintw(row++, 2, "Duration (MM:SS):");
	mvprintw(row++, 2, "Choir [X]:");
	mvprintw(row++, 2, "Singer Part:");
	mvprintw(row++, 2, "Instruments:");
	row += 2;
	attron(A_BOLD);
	mvprintw(row++, 2, "Scores:");
	attroff(A_BOLD);

	if (existing) {
		for (unsigned int i = 0; i < existing->getScores().size(); i++) {
			mvprintw(row++, 2, "%i) %s", i + 1,
					existing->getScores().at(i).getPath().c_str());
		}
	}

	row += 2;
	mvprintw(row, 2, "Press F2 for menu");

	refresh();
	form_driver(form, REQ_FIRST_FIELD);
}

void PieceForm::clearFormFields() {
    existing = nullptr;
    for (int i = 0; i < NUMBER_OF_FIELDS; ++i) {
        set_field_buffer(fields[i], 0, "");  // Svuota il contenuto del campo
    }
    hasChoiristChecked = false;
    set_field_buffer(fields[3], 0, "[ ]");  // Aggiorna il campo "Choir" a non selezionato

    form_driver(form, REQ_FIRST_FIELD);  // Posiziona il cursore sul primo campo
    refresh();
}


MenuCommand PieceForm::getCommand() {
	int ch;
	while ((ch = getch())) {
		switch (ch) {
		case KEY_F(2): { // MENU AZIONI
		    MenuCommand result = menuBar.show();
		    if (result == MenuCommand::AddPiece) {
		        validateFields();
		        clearFormFields();
		    } else {
		        validateFields();
		        return result;
		    }
		    break;
		}

		case KEY_DOWN:
		case '\t':
			form_driver(form, REQ_NEXT_FIELD);
			form_driver(form, REQ_END_LINE);
			break;
		case KEY_UP:
		case KEY_BTAB:
			form_driver(form, REQ_PREV_FIELD);
			form_driver(form, REQ_END_LINE);
			break;
		case 10: // ENTER (comportamento simile a TAB)
			form_driver(form, REQ_NEXT_FIELD);
			form_driver(form, REQ_END_LINE);
			break;
		case KEY_BACKSPACE:
		case 127:
		case '\b':
			form_driver(form, REQ_DEL_PREV);
			break;
		case KEY_DC:
			form_driver(form, REQ_DEL_CHAR);
			break;
		case ' ': // Barra spaziatrice per toggle
		    if (current_field(form) == fields[3]) {
		        hasChoiristChecked = !hasChoiristChecked;
		        set_field_buffer(fields[3], 0, hasChoiristChecked ? "[X]" : "[ ]");
		    }
		    break;
		default:
			form_driver(form, ch);
			break;
		}
		refresh();
	}
	return MenuCommand::None;
}

void PieceForm::validateFields() {
    form_driver(form, REQ_VALIDATION);

    std::string composer = trim(field_buffer(fields[0], 0));
    std::string title = trim(field_buffer(fields[1], 0));
    std::string durationStr = trim(field_buffer(fields[2], 0));
    int duration = convertToSeconds(durationStr);
    if (duration == -1)
        duration = 0;

    std::string singer = trim(field_buffer(fields[4], 0));
    std::string instruments = trim(field_buffer(fields[5], 0));
    bool choir = hasChoiristChecked;

    MusicalPiece p(composer, title, duration, choir, singer, instruments);
    if (existing && !existing->getScores().empty()) {
        p.setScores(existing->getScores());
    }
    pieces.push_back(p);
}

void PieceForm::closeForm() {
	unpost_form(form);
	free_form(form);
	for (int i = 0; i < NUMBER_OF_FIELDS; ++i)
		free_field(fields[i]);
}

std::vector<MusicalPiece> PieceForm::getPieces() {
	return pieces;
}
