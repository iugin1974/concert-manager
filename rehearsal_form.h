#pragma once

#include <string>
#include "MenuBar.h"
#include "Rehearsal.h"
#include "View.h"
#include <vector>
#include <form.h>
#define FORM_FIELDS 5

class RehearsalForm: public View {

public:
	void setMenuBar(const MenuBar &bar) override;
	MenuCommand show() override;
	~RehearsalForm() override = default;
	void setRehearsal(const Rehearsal *rehearsal);
	std::tm getDate() const;
	std::string getStartTime() const;
	std::string getPlace() const;
	std::string getMusicians() const;
	void validateFields() override;
	void closeForm() override;

private:
	const Rehearsal *existing = nullptr;
	std::tm date;
	std::string startTime;
	std::string place;
	std::string musicians;
	FIELD *fields[FORM_FIELDS];
	FORM *form;
};
