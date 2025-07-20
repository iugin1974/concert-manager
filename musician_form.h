#pragma once

#include <string>
#include "MenuBar.h"
#include "Musician.h"
#include "View.h"
#include <vector>
#include <form.h>
#define FORM_FIELDS 7

class MusicianForm: public View {

public:
	MenuCommand show() override;
	~MusicianForm() override = default;
	void setMusician(const Musician *musician);
	std::string getName();
	std::string getPhone();
	std::string getInstrument();
	std::string getMail();
	std::string getAddress();
	double getGage();
	void validateFields() override;
	void closeForm() override;

private:
	const Musician *existing = nullptr;
	std::string name;
	std::string phone;
	std::string instrument;
	std::string mail;
	std::string address;
	double gage;
	FIELD *fields[FORM_FIELDS];
	FORM *form;
};
