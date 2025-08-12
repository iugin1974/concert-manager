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
	void setRehearsal(Rehearsal *rehearsal);
	void updateFields() override;
	void init_form() override;
	void clearFormFields() override;

private:
	static constexpr int NUMBER_OF_FIELDS = 5;
	Rehearsal *rehearsal = nullptr;
	void closeForm() override;
	void saveDataFromForm() override;
	FIELD *fields[NUMBER_OF_FIELDS];
	FORM *form = nullptr;
};
