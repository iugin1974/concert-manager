#pragma once
#include "MusicalPiece.h"
#include "ConcertElementForm.h"

class PieceForm : public ConcertElementForm<MusicalPiece, 8> {
public:
    // Wrapper di compatibilità: il vero setter è setElement() nella base.
    void setPiece(MusicalPiece *p) { setElement(p); }

    void init_form() override;
    void updateFields() override;

private:
    void printLabels() override;
    void saveDataFromForm() override;
    bool handleSpecialKey(int ch) override;          // toggle spazio sul checkbox coro
    void onClearExtra() override;                     // reset hasChoiristChecked
    // shouldSaveOnCommand non overridato: usa il default della base
    // (salva su qualsiasi comando diverso da Quit, incluso AddScore/DeleteScore)

    bool hasChoiristChecked = false;
};
