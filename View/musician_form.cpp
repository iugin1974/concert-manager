#include "musician_form.h"
#include "Utils.h"
#include "ConcertController.h" // TODO elimina
#include <form.h>
#include <string>
#include <locale.h>
#include <sstream>

void MusicianForm::setController(ConcertController &c) {
// TODO elimina la dipendenza dal controller
	controller = c;
}

MusicianForm::~MusicianForm() {
	closeForm();
}

void MusicianForm::setMusician(Musician *m) {
	musician = m;
}

void MusicianForm::setAutoFilledFields(const Musician &m) {
	set_field_buffer(fields[1], 0, m.getPhone().c_str());
	set_field_buffer(fields[2], 0, m.getInstrument().c_str());
	set_field_buffer(fields[3], 0, m.getMail().c_str());
	set_field_buffer(fields[4], 0, m.getStreet().c_str());
	set_field_buffer(fields[5], 0, m.getZipCode().c_str());
	set_field_buffer(fields[6], 0, m.getCity().c_str());
}

void MusicianForm::updateFields() {
	if (!form || !musician)
		return;

	set_field_buffer(fields[0], 0, musician->getName().c_str());
	set_field_buffer(fields[1], 0, musician->getPhone().c_str());
	set_field_buffer(fields[2], 0, musician->getInstrument().c_str());
	set_field_buffer(fields[3], 0, musician->getMail().c_str());

	set_field_buffer(fields[4], 0, musician->getStreet().c_str());
	set_field_buffer(fields[5], 0, musician->getZipCode().c_str());
	set_field_buffer(fields[6], 0, musician->getCity().c_str());

	std::ostringstream oss;
	oss.precision(2);
	oss << std::fixed << musician->getGage();
	set_field_buffer(fields[7], 0, oss.str().c_str());

	set_field_buffer(fields[8], 0,
			std::to_string(musician->getRehearsalNumber()).c_str());
	set_field_buffer(fields[9], 0,
			std::to_string(musician->getConcertNumber()).c_str());

	isSoloistChecked = musician->isSoloist();
	set_field_buffer(fields[10], 0, isSoloistChecked ? "[X]" : "[ ]");

	set_field_buffer(fields[11], 0,
			std::to_string(musician->getTravelCosts()).c_str());

	refresh();
}

void MusicianForm::init_form() {
	if (form) return;
	int row = 2;
	// Crea i campi (posizionati su righe diverse)
	fields[0] = new_field(1, 40, row++, 30, 0, 0); // Name
	fields[1] = new_field(1, 40, row++, 30, 0, 0); // Phone
	fields[2] = new_field(1, 40, row++, 30, 0, 0); // Instrument
	fields[3] = new_field(1, 40, row++, 30, 0, 0); // Mail

	// Ora dividi indirizzo in tre campi:
	fields[4] = new_field(1, 40, row++, 30, 0, 0); // Street
	fields[5] = new_field(1, 10, row++, 30, 0, 0); // Zip Code
	fields[6] = new_field(1, 40, row++, 30, 0, 0); // City

	fields[7] = new_field(1, 40, row++, 30, 0, 0); // Gage

	row += 2;
	fields[8] = new_field(1, 2, row++, 30, 0, 0); // Numero prove
	fields[9] = new_field(1, 2, row++, 30, 0, 0); // Numero concerti
	fields[10] = new_field(1, 4, row++, 30, 0, 0); // Solista
	set_field_buffer(fields[10], 0, "[ ]");
	field_opts_off(fields[10], O_EDIT); // Non editabile manualmente
	fields[11] = new_field(1, 5, row++, 30, 0, 0); // Spese viaggio
	fields[12] = nullptr;

	for (int i = 0; i < 12; ++i) {
		set_field_back(fields[i], A_UNDERLINE);   // Campo visibile
		field_opts_off(fields[i], O_AUTOSKIP);  // Non salta automaticamente
	}

	// Crea e configura il form
	form = new_form(fields);
}

void MusicianForm::clearFormFields() {
	for (int i = 0; i < 12; ++i) {  // 11 campi più il nullptr a fields[12]
		set_field_buffer(fields[i], 0, "");  // Svuota il contenuto del campo
	}
	isSoloistChecked = false;
	set_field_buffer(fields[10], 0, "[ ]"); // Aggiorna il campo "Soloist" a non selezionato

	form_driver(form, REQ_FIRST_FIELD);  // Posiziona il cursore sul primo campo
	refresh();
}

void MusicianForm::show() {
	if (!form) init_form();
	post_form(form);

	int row = 2;
	mvprintw(row++, 2, "Name:");
	mvprintw(row++, 2, "Phone:");
	mvprintw(row++, 2, "Instrument:");
	mvprintw(row++, 2, "Mail:");
	mvprintw(row++, 2, "Street:");
	mvprintw(row++, 2, "ZIP Code:");
	mvprintw(row++, 2, "City:");
	mvprintw(row++, 2, "Gage:");
	row += 2;
	mvprintw(row++, 2, "Number of rehearsals");
	mvprintw(row++, 2, "Number of concerts");
	mvprintw(row++, 2, "Soloist (y/n)");
	mvprintw(row++, 2, "Travel cost (1 way)");
	row += 2;
	mvprintw(row++, 2, "Base salary:\t\t%4.2f", baseSalary);
	mvprintw(row++, 2, "Vacation compens.:\t%4.2f", vacationCompensation);
	mvprintw(row++, 2, "Insurances:\t\t%4.2f", insurances);
	attron(A_BOLD);
	mvprintw(row++, 2, "Salary:\t\t%4.2f", salary);
	attroff(A_BOLD);
	row++;
	mvprintw(row++, 2, "Travel costs:\t\t%4.2f", travelCosts);
	attron(A_BOLD);
	mvprintw(row++, 2, "Total costs:\t\t%4.2f", salary + travelCosts);
	attroff(A_BOLD);

	set_current_field(form, fields[0]);
	form_driver(form, REQ_FIRST_FIELD);
	form_driver(form, REQ_END_LINE);  // posiziona alla fine del buffer
	refresh();
}

MenuCommand MusicianForm::getCommand() {
	int ch;
	FIELD *prevField = current_field(form);
	while (true) {
		ch = getch();
		switch (ch) {
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
		case ' ': // Barra spaziatrice per toggle
			if (current_field(form) == fields[10]) {
				isSoloistChecked = !isSoloistChecked;
				set_field_buffer(fields[10], 0,
						isSoloistChecked ? "[X]" : "[ ]");
			} else {
				form_driver(form, ch);
			}
			break;
		case KEY_F(2): {
			MenuCommand result = menuBar.show();
			if (result != MenuCommand::Quit) {
				saveDataFromForm();
			}
			return result;
		}
		default:
			form_driver(form, ch);
			break;
		}
		FIELD *currField = current_field(form);

		if (currField != prevField) {
			handleFieldChange();  // Aggiungiamo questa funzione
			prevField = currField;
		}
		refresh();  // Aggiorna stdscr dopo ogni input

	}
	return MenuCommand::Quit;
}

void MusicianForm::saveDataFromForm() {
	form_driver(form, REQ_VALIDATION);

	musician->setName(trim(field_buffer(fields[0], 0)));
	musician->setPhone(trim(field_buffer(fields[1], 0)));
	musician->setInstrument(trim(field_buffer(fields[2], 0)));
	musician->setMail(trim(field_buffer(fields[3], 0)));
	musician->setStreet(trim(field_buffer(fields[4], 0)));
	musician->setZipCode(trim(field_buffer(fields[5], 0)));
	musician->setCity(trim(field_buffer(fields[6], 0)));

	std::string gage_str = trim(field_buffer(fields[7], 0));
	double gage = std::stod(gage_str.empty() ? "0" : gage_str);

	std::string rehearsalNumber_str = trim(field_buffer(fields[8], 0));
	int rehearsalNumber = std::stoi(
			rehearsalNumber_str.empty() ? "0" : rehearsalNumber_str);

	std::string concertNumber_str = trim(field_buffer(fields[9], 0));
	int concertNumber = std::stoi(
			concertNumber_str.empty() ? "0" : concertNumber_str);

	bool soloist = isSoloistChecked;

	std::string travelCosts_str = trim(field_buffer(fields[11], 0));
	int travelCosts = std::stoi(
			travelCosts_str.empty() ? "0" : travelCosts_str);

	musician->setGage(gage);
	musician->setRehearsalNumber(rehearsalNumber);
	musician->setConcertNumber(concertNumber);
	musician->setSoloist(soloist);
	musician->setTravelCosts(travelCosts);
}

void MusicianForm::closeForm() {
	unpost_form(form);
	free_form(form);
	for (int i = 0; fields[i] != nullptr; ++i) {
		free_field(fields[i]);
	}

}

void MusicianForm::handleFieldChange() {
	form_driver(form, REQ_VALIDATION);

	std::string name = trim(field_buffer(fields[0], 0));
	if (!name.empty()) {
		controller.autofillFromAbook(name, *this);
	}

	std::string prove_str = trim(field_buffer(fields[8], 0));    // Numero prove
	std::string concerti_str = trim(field_buffer(fields[9], 0)); // Numero concerti
	std::string soloista_str = trim(field_buffer(fields[10], 0)); // Solista (campo checkbox)
	std::string travelC_str = trim(field_buffer(fields[11], 0)); // Spese viaggio

	int prove = prove_str.empty() ? 0 : std::stoi(prove_str);
	int concerti = concerti_str.empty() ? 0 : std::stoi(concerti_str);
	bool soloist = (!soloista_str.empty()
			&& (soloista_str[0] == 'y' || soloista_str[0] == 'Y'
					|| soloista_str[0] == '1'));
	double travelC = travelC_str.empty() ? 0.0 : std::stod(travelC_str);

	Musician::SalaryDetails details = Musician::calculateSalary(prove, concerti,
			soloist, travelC);
	baseSalary = details.baseSalary;
	vacationCompensation = details.vacationCompensation;
	insurances = details.insurances;
	salary = details.totalSalary;
	travelCosts = details.travelCosts;

	int row = 2 + 8 + 2 + 3 + 2 + 1;
	mvprintw(row++, 2, "Base salary:\t\t%4.2f", baseSalary);
	mvprintw(row++, 2, "Vacation compens.:\t%4.2f", vacationCompensation);
	mvprintw(row++, 2, "Insurances:\t\t%4.2f", insurances);
	attron(A_BOLD);
	mvprintw(row++, 2, "Salary:\t\t%4.2f", salary);
	attroff(A_BOLD);
	row++;
	mvprintw(row++, 2, "Travel costs:\t\t%4.2f", travelCosts);
	attron(A_BOLD);
	mvprintw(row++, 2, "Total costs:\t\t%4.2f", salary + travelCosts);
	attroff(A_BOLD);
	pos_form_cursor(form); // posiziona il cursore
	refresh();
}
