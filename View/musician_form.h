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
	static constexpr int NUMBER_OF_FIELDS = 7;
	const Musician *existing = nullptr;
	void init_form() override;
	void closeForm() override;
	void validateFields() override;
	std::string name;
	std::string phone;
	std::string instrument;
	std::string mail;
	std::string address;
	double gage;
	FIELD *fields[NUMBER_OF_FIELDS];
	FORM *form = nullptr;
};
