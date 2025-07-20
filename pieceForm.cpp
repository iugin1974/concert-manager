#include <form.h>
#include <ncurses.h>
#include <string>
#include "pieceForm.h"
#include "Utils.h"

void PieceForm::setMenuBar(const MenuBar &bar) {
	menuBar = bar;
}

void PieceForm::setPiece(const MusicalPiece *piece) {
	existing = piece;
}

MenuCommand PieceForm::show() {
	// 1. Crea campi
	fields[0] = new_field(1, 40, 2, 20, 0, 0);  // Composer
	fields[1] = new_field(1, 40, 4, 20, 0, 0);  // Title
	fields[2] = new_field(1, 10, 8, 20, 0, 0);  // Duration (MM:SS)
	fields[3] = new_field(1, 1, 10, 20, 0, 0);  // Choir (Yes/No)
	fields[4] = new_field(1, 30, 12, 20, 0, 0); // Singer part
	fields[5] = new_field(1, 40, 14, 20, 0, 0); // Instruments
	fields[6] = nullptr;

	for (int i = 0; i < FORM_FIELDS; ++i) {
		set_field_back(fields[i], A_UNDERLINE);
		field_opts_off(fields[i], O_AUTOSKIP);
	}

	// 2. Pre-compila se già esistente
	if (existing) {
		set_field_buffer(fields[0], 0, existing->getComposer().c_str());
		set_field_buffer(fields[1], 0, existing->getTitle().c_str());
		set_field_buffer(fields[2], 0,
				convertToMMSS(existing->getDuration()).c_str());
		set_field_buffer(fields[3], 0, existing->hasChoir() ? "X" : "");
		set_field_buffer(fields[4], 0, existing->getSingerPart().c_str());
		set_field_buffer(fields[5], 0, existing->getInstruments().c_str());
	}

	form = new_form(fields);
	post_form(form);

	mvprintw(2, 2, "Composer:");
	mvprintw(4, 2, "Title*:");
	mvprintw(8, 2, "Duration (MM:SS):");
	mvprintw(10, 2, "Choir [X]:");
	mvprintw(12, 2, "Singer Part:");
	mvprintw(14, 2, "Instruments:");

	mvprintw(17, 2, "Press F2 for menu");

	refresh();
	form_driver(form, REQ_FIRST_FIELD);

	auto trim = [](std::string s) {
		s.erase(s.find_last_not_of(" \n") + 1);
		s.erase(0, s.find_first_not_of(" "));
		return s;
	};

	int ch;
	while ((ch = getch())) {
		switch (ch) {
		case KEY_F(2): {
			MenuCommand result = menuBar.show();
			return result;
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
		default:
			form_driver(form, ch);
			break;
		}
		refresh();
	}
}

void PieceForm::validateFields() {
	form_driver(form, REQ_VALIDATION);

	composer = trim(field_buffer(fields[0], 0));
	title = trim(field_buffer(fields[1], 0));
	std::string durationStr = trim(field_buffer(fields[2], 0));
	duration = convertToSeconds(durationStr); // se la conversione non e' possibile, ritorna -1
	if (duration == -1) duration = 0;  // allora va cambiato il valore a 0
	std::string choirStr = trim(field_buffer(fields[3], 0));
	singer = trim(field_buffer(fields[4], 0));
	instruments = trim(field_buffer(fields[5], 0));
	choir = !choirStr.empty();
}
void PieceForm::closeForm() {
	unpost_form(form);
	free_form(form);
	for (int i = 0; i < FORM_FIELDS; ++i)
		free_field(fields[i]);
}

const std::string PieceForm::getTitle() const { return title; }
const std::string PieceForm::getComposer() const { return composer; }
const int PieceForm::getDuration() const { return duration; }
const std::string PieceForm::getSinger() const { return singer; }
const std::string PieceForm::getInstruments() const { return instruments; }
const bool PieceForm::hasChoir() const { return choir; }
