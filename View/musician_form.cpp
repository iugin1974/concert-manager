#include "musician_form.h"
#include "Utils.h"
#include <sstream>
#include <ncurses.h>

void MusicianForm::setController(ConcertController &c) {
	// TODO elimina la dipendenza dal controller
	controller = c;
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
	if (!form || !element)
		return;

	set_field_buffer(fields[0], 0, element->getName().c_str());
	set_field_buffer(fields[1], 0, element->getPhone().c_str());
	set_field_buffer(fields[2], 0, element->getInstrument().c_str());
	set_field_buffer(fields[3], 0, element->getMail().c_str());

	set_field_buffer(fields[4], 0, element->getStreet().c_str());
	set_field_buffer(fields[5], 0, element->getZipCode().c_str());
	set_field_buffer(fields[6], 0, element->getCity().c_str());

	std::ostringstream oss;
	oss.precision(2);
	oss << std::fixed << element->getGage();
	set_field_buffer(fields[7], 0, oss.str().c_str());

	set_field_buffer(fields[8], 0,
			std::to_string(element->getRehearsalNumber()).c_str());
	set_field_buffer(fields[9], 0,
			std::to_string(element->getConcertNumber()).c_str());

	isSoloistChecked = element->isSoloist();
	set_field_buffer(fields[10], 0, isSoloistChecked ? "[X]" : "[ ]");

	set_field_buffer(fields[11], 0,
			std::to_string(element->getTravelCosts()).c_str());

	refresh();
}

void MusicianForm::init_form() {
	if (form) return;
	int row = 2;
	fields[0] = new_field(1, 40, row++, 30, 0, 0); // Name
	fields[1] = new_field(1, 40, row++, 30, 0, 0); // Phone
	fields[2] = new_field(1, 40, row++, 30, 0, 0); // Instrument
	fields[3] = new_field(1, 40, row++, 30, 0, 0); // Mail

	fields[4] = new_field(1, 40, row++, 30, 0, 0); // Street
	fields[5] = new_field(1, 10, row++, 30, 0, 0); // Zip Code
	fields[6] = new_field(1, 40, row++, 30, 0, 0); // City

	fields[7] = new_field(1, 40, row++, 30, 0, 0); // Gage

	row += 2;
	fields[8] = new_field(1, 2, row++, 30, 0, 0);  // Numero prove
	fields[9] = new_field(1, 2, row++, 30, 0, 0);  // Numero concerti
	fields[10] = new_field(1, 4, row++, 30, 0, 0); // Solista
	set_field_buffer(fields[10], 0, "[ ]");
	field_opts_off(fields[10], O_EDIT); // Non editabile manualmente
	fields[11] = new_field(1, 5, row++, 30, 0, 0); // Spese viaggio
	fields[12] = nullptr;

	for (int i = 0; i < 12; ++i) {
		set_field_back(fields[i], A_UNDERLINE);
		field_opts_off(fields[i], O_AUTOSKIP);
	}

	form = new_form(fields);
}

void MusicianForm::clearFormFields() {
	ConcertElementForm<Musician, 13>::clearFormFields(); // svuota i campi + chiama onClearExtra()
}

void MusicianForm::onClearExtra() {
	isSoloistChecked = false;
	set_field_buffer(fields[10], 0, "[ ]");
}

void MusicianForm::printLabels() {
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
}

bool MusicianForm::handleSpecialKey(int ch) {
	if (ch == ' ' && current_field(form) == fields[10]) {
		isSoloistChecked = !isSoloistChecked;
		set_field_buffer(fields[10], 0, isSoloistChecked ? "[X]" : "[ ]");
		return true; // gestito qui, il driver comune non deve toccare questo tasto
	}
	return false;
}

void MusicianForm::onFieldChanged() {
	form_driver(form, REQ_VALIDATION);

	std::string name = trim(field_buffer(fields[0], 0));
	if (!name.empty()) {
		controller.autofillFromAbook(name, *this);
	}

	std::string prove_str = trim(field_buffer(fields[8], 0));
	std::string concerti_str = trim(field_buffer(fields[9], 0));
	std::string soloista_str = trim(field_buffer(fields[10], 0));
	std::string travelC_str = trim(field_buffer(fields[11], 0));

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
	pos_form_cursor(form);
	refresh();
}

void MusicianForm::saveDataFromForm() {
	form_driver(form, REQ_VALIDATION);

	element->setName(trim(field_buffer(fields[0], 0)));
	element->setPhone(trim(field_buffer(fields[1], 0)));
	element->setInstrument(trim(field_buffer(fields[2], 0)));
	element->setMail(trim(field_buffer(fields[3], 0)));
	element->setStreet(trim(field_buffer(fields[4], 0)));
	element->setZipCode(trim(field_buffer(fields[5], 0)));
	element->setCity(trim(field_buffer(fields[6], 0)));

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

	element->setGage(gage);
	element->setRehearsalNumber(rehearsalNumber);
	element->setConcertNumber(concertNumber);
	element->setSoloist(soloist);
	element->setTravelCosts(travelCosts);
}
