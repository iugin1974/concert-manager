#include <form.h>
#include <ncurses.h>
#include <string>
#include <vector>
#include <sstream>
#include "Concert.h"
#include "concert_info_form.h"
#include "Utils.h"
#include "logMessage.h"

ConcertInfoForm::ConcertInfoForm() {
}

ConcertInfoForm::~ConcertInfoForm() {
	closeForm();
}

void ConcertInfoForm::setConcert(const Concert *existing) {
	this->existing = existing;
}

void ConcertInfoForm::init_form() {
	if (form == nullptr) {

		int row = 5;
		fields[0] = new_field(1, 40, row++, 30, 0, 0); // Title
		fields[1] = new_field(1, 40, row++, 30, 0, 0); // Places
		fields[2] = new_field(1, 40, row++, 30, 0, 0); // Dates
		fields[3] = nullptr;

		for (int i = 0; i < NUMBER_OF_FIELDS - 1; ++i) {
			set_field_back(fields[i], A_UNDERLINE);
			field_opts_off(fields[i], O_AUTOSKIP);
		}
	}
	if (existing) {
		set_field_buffer(fields[0], 0, existing->getTitle().c_str());

		std::string placesStr, datesStr;
		const auto &pl = existing->getPlaces();
		const auto &dt = existing->getDatesAsString();

		for (size_t i = 0; i < pl.size(); ++i)
			placesStr += (i ? ", " : "") + pl[i];

		for (size_t i = 0; i < dt.size(); ++i)
			datesStr += (i ? ", " : "") + dt[i];

		set_field_buffer(fields[1], 0, placesStr.c_str());
		set_field_buffer(fields[2], 0, datesStr.c_str());
	}

	form = new_form(fields);
}
void ConcertInfoForm::show() {

	init_form();
	post_form(form);

	int row = 2;
	attron(A_BOLD);
	mvprintw(row++, 2, "Concert Info:");
	attroff(A_BOLD);
	row += 2;
	mvprintw(row++, 2, "Title:");
	mvprintw(row++, 2, "Places (comma-separated):");
	mvprintw(row++, 2, "Dates  (comma-separated):");

	set_current_field(form, fields[0]);
	form_driver(form, REQ_FIRST_FIELD);
	form_driver(form, REQ_END_LINE);  // posiziona alla fine del buffer
	refresh();
}

MenuCommand ConcertInfoForm::getCommand() {
	int ch;
	while ((ch = getch())) {
		switch (ch) {
		case KEY_F(2): {
			saveDataFromForm();
			MenuCommand result = menuBar.show();
			return result;
		}
		case KEY_DOWN:
		case 9:
			form_driver(form, REQ_NEXT_FIELD);
			form_driver(form, REQ_END_LINE);
			break;
		case KEY_UP:
		case KEY_BTAB:
			form_driver(form, REQ_PREV_FIELD);
			form_driver(form, REQ_END_LINE);
			break;
		case 10:
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

	return MenuCommand::Quit;
}

void ConcertInfoForm::saveDataFromForm() {
	form_driver(form, REQ_VALIDATION);
	title = trim(field_buffer(fields[0], 0));
	places = trim(field_buffer(fields[1], 0));
	dates = trim(field_buffer(fields[2], 0));
}

void ConcertInfoForm::closeForm() {
	unpost_form(form);
	free_form(form);
	for (int i = 0; fields[i]; ++i)
		free_field(fields[i]);
}

// Helper: split da stringa separata da virgole
std::vector<std::string> ConcertInfoForm::split(const std::string &input,
		char sep = ',') {
	std::stringstream ss(input);
	std::string item;
	std::vector<std::string> result;
	while (std::getline(ss, item, sep)) {
		result.push_back(trim(item.c_str()));
	}
	return result;
}

const std::string ConcertInfoForm::getDates() const {
	return dates;
}
const std::vector<std::string> ConcertInfoForm::getDatesAsVector() {
	return split(dates, ',');
}
const std::vector<std::string> ConcertInfoForm::getPlacesAsVector() {
	return split(places, ',');
}
const std::string ConcertInfoForm::getPlaces() const {
	return places;
}
const std::string ConcertInfoForm::getTitle() const {
	return title;
}
