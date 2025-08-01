#pragma once

#include <string>
#include "MenuBar.h"
#include "Rehearsal.h"
#include "View.h"
#include <vector>
#include <form.h>

class RehearsalForm: public View {

public:
	void setMenuBar(const MenuBar &bar) override;
	void show() override;
	    MenuCommand getCommand() override;
	~RehearsalForm() override;
	void setRehearsal(const Rehearsal *rehearsal);
	std::vector<Rehearsal> getRehearsals() const;

private:
	static constexpr int NUMBER_OF_FIELDS = 5;
	const Rehearsal *existing = nullptr;
	void init_form() override;
	void closeForm() override;
	void saveDataFromForm() override;
	void clearFormFields();
	std::vector<Rehearsal> rehearsals;
	FIELD *fields[NUMBER_OF_FIELDS];
	FORM *form = nullptr;
};
