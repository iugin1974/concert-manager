#include "musician_form.h"
#include "Utils.h"
#include <form.h>
#include <string>
#include <locale.h>

MusicianForm::~MusicianForm() {
closeForm();
}

void MusicianForm::setMusician(const Musician *m) {
	existing = m;
}

void MusicianForm::init_form() {
	if (form != nullptr) return;
	int row = 2;
	// Crea i campi (posizionati su righe diverse)
	fields[0] = new_field(1, 40, row++, 30, 0, 0); // Name
	fields[1] = new_field(1, 40, row++, 30, 0, 0); // Phone
	fields[2] = new_field(1, 40, row++, 30, 0, 0); // Instrument
	fields[3] = new_field(1, 40, row++, 30, 0, 0); // Mail
	fields[4] = new_field(1, 40, row++, 30, 0, 0); // Address
	fields[5] = new_field(1, 40, row++, 30, 0, 0); // Gage
	row +=2;
	fields[6] = new_field(1, 2, row++, 30, 0, 0); // Numero prove
	fields[7] = new_field(1, 2, row++, 30, 0, 0); // Numero concerti
	fields[8] = new_field(1, 1, row++, 30, 0, 0); // Solista
	fields[9] = nullptr;

	for (int i = 0; i < 9; ++i) {
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
}

void MusicianForm::show() {
    init_form();
    post_form(form);
    refresh();

    int row = 2;
    mvprintw(row++, 2, "Name:");
    mvprintw(row++, 2, "Phone:");
    mvprintw(row++, 2, "Instrument:");
    mvprintw(row++, 2, "Mail:");
    mvprintw(row++, 2, "Address:");
    mvprintw(row++, 2, "Gage:");
    row += 2;
    mvprintw(row++, 2, "Number of rehearsals");
    mvprintw(row++, 2, "Number of concerts");
    mvprintw(row++, 2, "Soloist (y/n)");
    row += 2;
    mvprintw(row++, 2, "Base salary: %.2f", baseSalary);
    mvprintw(row++, 2, "Vacation compensation: %.2f", vacationCompensation);
    mvprintw(row++, 2, "Insurances: %.2f", insurances);
    mvprintw(row++, 2, "Salary: %.2f", salary);

    refresh();

    form_driver(form, REQ_FIRST_FIELD);
}

MenuCommand MusicianForm::getCommand() {
	int ch;
	FIELD* prevField = current_field(form);
	while (true) {
		ch = getch();
		switch (ch) {
		case KEY_F(2): { // MENU AZIONI
			validateFields();
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
		FIELD* currField = current_field(form);

				if (currField != prevField) {
				    handleFieldChange(prevField);  // Aggiungiamo questa funzione
				    prevField = currField;
				}
		refresh();  // Aggiorna stdscr dopo ogni input

	}
}

void MusicianForm::validateFields() {
	form_driver(form, REQ_VALIDATION);

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
	for (int i = 0; i < 9; ++i)
		free_field(fields[i]);

}

void MusicianForm::handleFieldChange(FIELD* field) {
    int index = field_index(field);

    if (index == 6 || index == 7 || index == 8) { // anche campo solista (bool)
        form_driver(form, REQ_VALIDATION);

        std::string prove_str = trim(field_buffer(fields[6], 0));
        std::string concerti_str = trim(field_buffer(fields[7], 0));
        std::string soloista_str = trim(field_buffer(fields[8], 0));

        int prove = prove_str.empty() ? 0 : std::stoi(prove_str);
        int concerti = concerti_str.empty() ? 0 : std::stoi(concerti_str);
        bool soloist = (!soloista_str.empty() && (soloista_str[0] == 'y' || soloista_str[0] == 'Y' || soloista_str[0] == '1'));

        Musician::SalaryDetails details = Musician::calculateSalary(prove, concerti, soloist);
        baseSalary = details.baseSalary;
        vacationCompensation = details.vacationCompensation;
        insurances = details.insurances;
        salary = details.totalSalary;

        int row = 2 + 6 + 2 + 3 + 2;
            mvprintw(row++, 2, "Base salary: %.2f", baseSalary);
            mvprintw(row++, 2, "Vacation compensation: %.2f", vacationCompensation);
            mvprintw(row++, 2, "Insurances: %.2f", insurances);
            mvprintw(row++, 2, "Salary: %.2f", salary);
            refresh();

    }
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
