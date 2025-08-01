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
	std::vector<MusicalPiece> getPieces();

private:
	static constexpr int NUMBER_OF_FIELDS = 7;
	const MusicalPiece *existing = nullptr;
	void init_form() override;
	void closeForm() override;
	void saveDataFromForm() override;
	void clearFormFields();
	std::vector<MusicalPiece> pieces;
	FIELD *fields[NUMBER_OF_FIELDS];
	FORM *form = nullptr;
	bool hasChoiristChecked = false;
};
