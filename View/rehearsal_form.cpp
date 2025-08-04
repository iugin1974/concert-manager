#include "rehearsal_form.h"
#include <form.h>
#include <ncurses.h>
#include <string>
#include <vector>
#include "Utils.h" // trim, dateToString, stringToDate
#include <ctime>
#include <form.h>

RehearsalForm::~RehearsalForm() {
	closeForm();
}

void RehearsalForm::setMenuBar(const MenuBar &bar) {
	menuBar = bar;
}

void RehearsalForm::setRehearsal(const Rehearsal *rehearsal) {
	existing = rehearsal;
}

void RehearsalForm::init_form() {
	if (form == nullptr) {

		int row = 2;
		fields[0] = new_field(1, 20, row++, 25, 0, 0); // Date
		fields[1] = new_field(1, 10, row++, 25, 0, 0); // Start time
		fields[2] = new_field(1, 40, row++, 25, 0, 0); // Place
		fields[3] = new_field(1, 40, row++, 25, 0, 0); // Musicians
		fields[4] = nullptr;

		for (int i = 0; i < 4; ++i) {
			set_field_back(fields[i], A_UNDERLINE);
			field_opts_off(fields[i], O_AUTOSKIP);
		}

		if (existing) {
			set_field_buffer(fields[0], 0,
					dateToString(existing->getDate()).c_str());
			set_field_buffer(fields[1], 0, existing->getStartTime().c_str());
			set_field_buffer(fields[2], 0, existing->getPlace().c_str());
			set_field_buffer(fields[3], 0, existing->getMusicians().c_str());
		}

		form = new_form(fields);
	}
}

void RehearsalForm::show() {
	init_form();
	post_form(form);

	int row = 2;
	mvprintw(row++, 2, "Date (DD.MM.YYYY):");
	mvprintw(row++, 2, "Start time (HH:MM):");
	mvprintw(row++, 2, "Place:");
	mvprintw(row++, 2, "Musicians:");

	set_current_field(form, fields[0]);
	form_driver(form, REQ_FIRST_FIELD);
	form_driver(form, REQ_END_LINE);  // posiziona alla fine del buffer
	refresh();

}

MenuCommand RehearsalForm::getCommand() {
	int ch;
	bool done = false;
	while (!done) {
		ch = getch();
		switch (ch) {
		case 10:
			form_driver(form, REQ_NEXT_FIELD);
			form_driver(form, REQ_END_LINE);
			break;
		case KEY_DOWN:
		case 9:
			form_driver(form, REQ_NEXT_FIELD);
			break;
		case KEY_UP:
			form_driver(form, REQ_PREV_FIELD);
			break;
		case KEY_BACKSPACE:
		case 127:
			form_driver(form, REQ_DEL_PREV);
			break;
		case KEY_F(2): {
			MenuCommand result = menuBar.show();
			if (result == MenuCommand::AddRehearsal) {
				saveDataFromForm();
				clearFormFields();
			} else {
				saveDataFromForm();
			return result;
			}
		}
			break;
		default:
			form_driver(form, ch);
			break;
		}
	}
	return MenuCommand::Quit;
}

void RehearsalForm::clearFormFields() {
	existing = nullptr;
	for (int i = 0; i < 4; ++i) {
		set_field_buffer(fields[i], 0, "");
	}
	form_driver(form, REQ_FIRST_FIELD);  // Posiziona il cursore sul primo campo
	    refresh();
}

void RehearsalForm::saveDataFromForm() {
	form_driver(form, REQ_VALIDATION);
	std::string dateStr = trim(field_buffer(fields[0], 0));
	std::tm date;
	if (!stringToDate(dateStr, date)) {
		mvprintw(12, 2, "Invalid date format. Press any key.");
		getch();
		return; // TODO
	}

	std::string startTime = trim(field_buffer(fields[1], 0));
	std::string place = trim(field_buffer(fields[2], 0));
	std::string musicians = trim(field_buffer(fields[3], 0));

	Rehearsal r(date, startTime, place, musicians);
	rehearsals.push_back(r);
}

void RehearsalForm::closeForm() {
	unpost_form(form);
	free_form(form);
	for (int i = 0; fields[i] != nullptr; ++i) {
	    free_field(fields[i]);
	}
}

std::vector<Rehearsal> RehearsalForm::getRehearsals() const {
	return rehearsals;
}
