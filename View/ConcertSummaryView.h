// ConcertSummaryView.h
#pragma once

#include "Concert.h"
#include "View.h"

class ConcertSummaryView: public View {
public:
	void show() override;
	    MenuCommand getCommand() override;
	~ConcertSummaryView() override;
	void setConcert(const Concert &concert);
	void updateFields() override;
	void clearFormFields() override;

private:
	int firstLine = 1;
	Concert concert;
	void init_form() override;
	void closeForm() override;
	void saveDataFromForm() override;


};

