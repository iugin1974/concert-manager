#pragma once

#include <string>
#include "MenuBar.h"
#include "Musician.h"
#include "ConcertController.h" // TODO elimina
#include "View.h"
#include <vector>
#include <form.h>

class MusicianForm: public View {

public:
	void show() override;
	MenuCommand getCommand() override;
	~MusicianForm() override;
	void init_form() override;
	void setController(ConcertController& controller); // TODO elimina
	void setMusician(Musician *musician);
	std::string getName();
	std::string getPhone();
	std::string getInstrument();
	std::string getMail();
	std::string getAddress();
	int getRehearsalNumber() const;
	int getConcertNumber() const;
	double getTravelCosts() const;
	double getGage() const;
	bool getSoloist() const;
	void setAutoFilledFields(const Musician& m);
	void updateFields() override;
	void clearFormFields() override;

private:
	static constexpr int NUMBER_OF_FIELDS = 13;
	Musician *musician = nullptr;
	void closeForm() override;
	void saveDataFromForm() override;
	void handleFieldChange();
	
	std::string name;
	std::string phone;
	std::string instrument;
	std::string mail;
	std::string address;
	double baseSalary = 0.0;
	double vacationCompensation = 0.0;
	double insurances = 0.0;
	double salary = 0.0;
	double travelCosts = 0.0;
	int rehearsalNumber = 0;
	int concertNumber = 0;
	bool soloist = false;
	double gage;
	bool isSoloistChecked = false;

	ConcertController controller;
	FIELD *fields[NUMBER_OF_FIELDS];
	FORM *form = nullptr;
};
