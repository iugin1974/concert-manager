#pragma once
#include "Rehearsal.h"
#include "ConcertElementForm.h"

class RehearsalForm : public ConcertElementForm<Rehearsal, 5> {
public:
    // Wrapper di compatibilità: il vero setter è setElement() nella base.
    // Valuta se aggiornare i call site ed eliminare questo metodo.
    void setRehearsal(Rehearsal *r) { setElement(r); }

    void updateFields() override;
    void init_form() override;

private:
    void printLabels() override;
    void saveDataFromForm() override;
};
