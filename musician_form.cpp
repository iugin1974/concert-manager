#include "musician_form.h"
#include <form.h>
#include <string>
#include <locale.h>

void MusicianForm::setMusician(const Musician *m) {
	existing = m;
}

MenuCommand MusicianForm::show() {

	// Crea i campi (posizionati su righe diverse)
	fields[0] = new_field(1, 40, 2, 20, 0, 0); // Name
	fields[1] = new_field(1, 40, 4, 20, 0, 0); // Phone
	fields[2] = new_field(1, 40, 6, 20, 0, 0); // Instrument
	fields[3] = new_field(1, 40, 8, 20, 0, 0); // Mail
	fields[4] = new_field(1, 40, 10, 20, 0, 0); // Address
	fields[5] = new_field(1, 40, 12, 20, 0, 0); // Gage
	fields[6] = nullptr;

	for (int i = 0; i < 6; ++i) {
		set_field_back(fields[i], A_UNDERLINE);   // Campo visibile
		field_opts_off(fields[i], O_AUTOSKIP);    // Non salta automaticamente
	}

	// Precompilazione se "existing" presente
	if (existing) {
		set_field_buffer(fields[0], 0, existing->getName().c_str());
		set_field_buffer(fields[1], 0, existing->getPhone().c_str());
		set_field_buffer(fields[2], 0, existing->getInstrument().c_str());
		set_field_buffer(fields[3], 0, existing->getMail().c_str());
		set_field_buffer(fields[4], 0, existing->getAddress().c_str());
		set_field_buffer(fields[5], 0,
				std::to_string(existing->getGage()).c_str());
	}

	// Crea e configura il form
	form = new_form(fields);
	post_form(form);
	refresh(); // stdscr

	// Etichette
	mvprintw(2, 2, "Name:");
	mvprintw(4, 2, "Phone:");
	mvprintw(6, 2, "Instrument:");
	mvprintw(8, 2, "Mail:");
	mvprintw(10, 2, "Address:");
	mvprintw(12, 2, "Gage:");
	mvprintw(14, 2, "Press ENTER to confirm");

	refresh();

	form_driver(form, REQ_FIRST_FIELD);

	int ch;
	while (true) {
		ch = getch();
		switch (ch) {
		case KEY_F(2): { // MENU AZIONI
			MenuCommand result = menuBar.show();
			return result;
			break;
		}
		case 10: // Enter
		case KEY_DOWN:
		case 9: // TAB
			form_driver(form, REQ_NEXT_FIELD);
			form_driver(form, REQ_END_LINE);
			break;
		case KEY_UP:
		case KEY_BTAB:
			form_driver(form, REQ_PREV_FIELD);
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
		refresh();  // Aggiorna stdscr dopo ogni input
	}
}

void MusicianForm::validateFields() {
	form_driver(form, REQ_VALIDATION);

	// Estrai i dati
	auto trim = [](std::string s) {
		s.erase(s.find_last_not_of(" \n") + 1);
		s.erase(0, s.find_first_not_of(" "));
		return s;
	};

	name = trim(field_buffer(fields[0], 0));
	phone = trim(field_buffer(fields[1], 0));
	instrument = trim(field_buffer(fields[2], 0));
	mail = trim(field_buffer(fields[3], 0));
	address = trim(field_buffer(fields[4], 0));
	std::string gage_str = trim(field_buffer(fields[5], 0));
	gage = std::stod(gage_str.empty() ? "0" : gage_str);
}

void MusicianForm::closeForm() {
	unpost_form(form);
	free_form(form);
	for (int i = 0; i < 6; ++i)
		free_field(fields[i]);

}

std::string MusicianForm::getName() {
	return name;
}
std::string MusicianForm::getPhone() {
	return phone;
}
std::string MusicianForm::getInstrument() {
	return instrument;
}
std::string MusicianForm::getMail() {
	return mail;
}
std::string MusicianForm::getAddress() {
	return address;
}
double MusicianForm::getGage() {
	return gage;
}
