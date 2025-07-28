#pragma once

#include <string>
#include "MenuBar.h"
#include "Musician.h"
#include "View.h"
#include <vector>
#include <form.h>

class MusicianForm: public View {

public:
	void show() override;
	MenuCommand getCommand() override;
	~MusicianForm() override;
	void setMusician(const Musician *musician);
	std::string getName();
	std::string getPhone();
	std::string getInstrument();
	std::string getMail();
	std::string getAddress();
	double getGage();

private:
	static constexpr int NUMBER_OF_FIELDS = 11;
	const Musician *existing = nullptr;
	void init_form() override;
	void closeForm() override;
	void validateFields() override;
	void handleFieldChange(FIELD* field);
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
	double gage;
	FIELD *fields[NUMBER_OF_FIELDS];
	FORM *form = nullptr;
};
