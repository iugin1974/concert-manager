#include "rehearsal_form.h"
#include "Utils.h" // trim, dateToString, stringToDate
#include <ctime>

void RehearsalForm::updateFields() {
	if (!form || !element)
		return;
	std::string dateStr = dateToString(element->getDate());
	if (dateStr == "00.01.1900") {
		dateStr = ""; // Mostra campo vuoto se la data è quella di default
	}
	set_field_buffer(fields[0], 0, dateStr.c_str());
	set_field_buffer(fields[1], 0, element->getStartTime().c_str());
	set_field_buffer(fields[2], 0, element->getPlace().c_str());
	set_field_buffer(fields[3], 0, element->getMusicians().c_str());
	refresh();
}

void RehearsalForm::init_form() {
	if (form) return;
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
	form = new_form(fields);
}

void RehearsalForm::printLabels() {
	int row = 2;
	mvprintw(row++, 2, "Date (DD.MM.YYYY):");
	mvprintw(row++, 2, "Start time (HH:MM):");
	mvprintw(row++, 2, "Place:");
	mvprintw(row++, 2, "Musicians:");
}

void RehearsalForm::saveDataFromForm() {
	form_driver(form, REQ_VALIDATION);
	std::string dateStr = trim(field_buffer(fields[0], 0));
	std::tm date;
	if (!stringToDate(dateStr, date)) {
		mvprintw(12, 2, "Invalid date format. Press any key.");
		getch();
		return;
	}
	element->setDate(date);
	element->setStartTime(trim(field_buffer(fields[1], 0)));
	element->setPlace(trim(field_buffer(fields[2], 0)));
	element->setMusicians(trim(field_buffer(fields[3], 0)));
}
