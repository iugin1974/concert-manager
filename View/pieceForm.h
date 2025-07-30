#pragma once
#include "MusicalPiece.h"
#include "View.h"

class PieceForm: public View {

public:
	void setMenuBar(const MenuBar &bar) override;
	void show() override;
	MenuCommand getCommand() override;
	~PieceForm() override;
	void setPiece(const MusicalPiece *piece);
	const std::string getTitle() const;
	const std::string getComposer() const;
	int getDuration() const;
	const std::string getSinger() const;
	const std::string getInstruments() const;
	bool hasChoir() const;

private:
	static constexpr int NUMBER_OF_FIELDS = 7;
	const MusicalPiece *existing = nullptr;
	void init_form() override;
	void closeForm() override;
	void validateFields() override;
	std::string title;
	std::string composer;
	int duration = 0;
	std::string singer;
	std::string instruments;
	bool choir = false;
	FIELD *fields[NUMBER_OF_FIELDS];
	FORM *form = nullptr;
	bool hasChoiristChecked = false;
};
