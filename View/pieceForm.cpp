#include "pieceForm.h"
#include "Utils.h"

void PieceForm::updateFields() {
	if (!form || !element)
		return;

	set_field_buffer(fields[0], 0, element->getComposer().c_str());
	set_field_buffer(fields[1], 0, element->getTitle().c_str());
	set_field_buffer(fields[2], 0, convertToMMSS(element->getDuration()).c_str());
	hasChoiristChecked = element->hasChoir();
	set_field_buffer(fields[3], 0, hasChoiristChecked ? "[X]" : "[ ]");
	set_field_buffer(fields[4], 0, element->getSingerPart().c_str());
	set_field_buffer(fields[5], 0, element->getInstruments().c_str());
	set_field_buffer(fields[6], 0, element->getYoutubeLink().c_str());

	set_current_field(form, fields[0]);
	form_driver(form, REQ_FIRST_FIELD);
	form_driver(form, REQ_END_LINE);
	refresh();
}

void PieceForm::init_form() {
	if (form != nullptr)
		closeForm();

	int row = 2;
	fields[0] = new_field(1, 40, row++, 20, 0, 0);  // Composer
	fields[1] = new_field(1, 40, row++, 20, 0, 0);  // Title
	fields[2] = new_field(1, 10, row++, 20, 0, 0);  // Duration (MM:SS)
	fields[3] = new_field(1, 4, row++, 20, 0, 0);   // Choir (Yes/No)
	set_field_buffer(fields[3], 0, "[ ]");
	field_opts_off(fields[3], O_EDIT); // Non editabile manualmente
	fields[4] = new_field(1, 40, row++, 20, 0, 0);  // Singer part
	fields[5] = new_field(1, 40, row++, 20, 0, 0);  // Instruments
	fields[6] = new_field(1, 140, row++, 20, 0, 0); // YouTube link
	fields[7] = nullptr;

	for (int i = 0; i < NUMBER_OF_FIELDS; ++i) {
		set_field_back(fields[i], A_UNDERLINE);
		field_opts_off(fields[i], O_AUTOSKIP);
	}

	form = new_form(fields);
}

void PieceForm::printLabels() {
	int row = 2;
	mvprintw(row++, 2, "Composer:");
	mvprintw(row++, 2, "Title*:");
	mvprintw(row++, 2, "Duration (MM:SS):");
	mvprintw(row++, 2, "Choir [X]:");
	mvprintw(row++, 2, "Singer Part:");
	mvprintw(row++, 2, "Instruments:");
	mvprintw(row++, 2, "YouTube Link:");
	row += 2;
	attron(A_BOLD);
	mvprintw(row++, 2, "Scores:");
	attroff(A_BOLD);

	if (element) {
		for (unsigned int i = 0; i < element->getScores().size(); i++) {
			mvprintw(row++, 2, "%i) %s", i + 1,
					element->getScores().at(i).getPath().c_str());
		}
	}
}

bool PieceForm::handleSpecialKey(int ch) {
	if (ch == ' ' && current_field(form) == fields[3]) {
		hasChoiristChecked = !hasChoiristChecked;
		set_field_buffer(fields[3], 0, hasChoiristChecked ? "[X]" : "[ ]");
		return true;
	}
	return false;
}

void PieceForm::onClearExtra() {
	hasChoiristChecked = false;
	set_field_buffer(fields[3], 0, "[ ]");
}

void PieceForm::saveDataFromForm() {
	form_driver(form, REQ_VALIDATION);

	element->setComposer(trim(field_buffer(fields[0], 0)));
	element->setTitle(trim(field_buffer(fields[1], 0)));
	std::string durationStr = trim(field_buffer(fields[2], 0));
	int duration = convertToSeconds(durationStr);
	if (duration == -1)
		duration = 0;
	element->setDuration(duration);
	element->setSingerPart(trim(field_buffer(fields[4], 0)));
	element->setInstruments(trim(field_buffer(fields[5], 0)));
	element->setYoutubeLink(trim(field_buffer(fields[6], 0)));
	element->setChoir(hasChoiristChecked);
}
