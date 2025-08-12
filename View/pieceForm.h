#pragma once
#include "MusicalPiece.h"
#include "View.h"

class PieceForm: public View {

public:
	void setMenuBar(const MenuBar &bar) override;
	void show() override;
	MenuCommand getCommand() override;
	~PieceForm() override;
	void setPiece(MusicalPiece *piece);
	void updateFields() override;
	void clearFormFields() override;
	void init_form() override;
private:
	static constexpr int NUMBER_OF_FIELDS = 8;
	MusicalPiece *piece = nullptr;
	void closeForm() override;
	void saveDataFromForm() override;
	FIELD *fields[NUMBER_OF_FIELDS];
	FORM *form = nullptr;
	bool hasChoiristChecked = false;
};
