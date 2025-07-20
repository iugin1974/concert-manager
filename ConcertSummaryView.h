// ConcertSummaryView.h
#pragma once

#include "Concert.h"
#include "View.h"

class ConcertSummaryView: public View {
public:
	MenuCommand show() override;
	~ConcertSummaryView() override = default;
	void setConcert(const Concert &concert);
	void validateFields() override;
	void closeForm() override;

private:
	Concert concert;

};

