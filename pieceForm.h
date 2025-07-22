#pragma once
#include "MusicalPiece.h"
#include "View.h"

class PieceForm : public View {

public:
	void setMenuBar(const MenuBar &bar) override;
	MenuCommand show() override;
	~PieceForm() override = default;
	void setPiece(const MusicalPiece *piece);
	const std::string getTitle() const;
	const std::string getComposer() const;
	int getDuration() const;
	const std::string getSinger() const;
	const std::string getInstruments() const;
	bool hasChoir() const;
	void validateFields() override;
	void closeForm() override;

private:
	static constexpr int NUMBER_OF_FIELDS = 7;
	const MusicalPiece *existing = nullptr;
	std::string title;
	std::string composer;
	int duration = 0;
	std::string singer;
	std::string instruments;
	bool choir = false;
	FIELD *fields[NUMBER_OF_FIELDS];
	FORM *form;
};
