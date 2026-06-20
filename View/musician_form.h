#pragma once
#include <string>
#include "Musician.h"
#include "ConcertController.h" // TODO elimina
#include "ConcertElementForm.h"

class MusicianForm : public ConcertElementForm<Musician, 13> {
public:
    // Wrapper di compatibilità: il vero setter è setElement() nella base.
    void setMusician(Musician *m) { setElement(m); }

    void init_form() override;
    void updateFields() override;
    void clearFormFields() override;     // serve override extra: resetta isSoloistChecked

    void setController(ConcertController &controller); // TODO elimina
    void setAutoFilledFields(const Musician &m);

    std::string getName();
    std::string getPhone();
    std::string getInstrument();
    std::string getMail();
    std::string getAddress();
    int getRehearsalNumber() const;
    int getConcertNumber() const;
    double getTravelCosts() const;
    double getGage() const;
    bool getSoloist() const;

private:
    void printLabels() override;
    void saveDataFromForm() override;
    bool handleSpecialKey(int ch) override;   // toggle spazio sul checkbox solista
    void onFieldChanged() override;           // ex handleFieldChange()
    void onClearExtra() override;             // reset isSoloistChecked

    double baseSalary = 0.0;
    double vacationCompensation = 0.0;
    double insurances = 0.0;
    double salary = 0.0;
    double travelCosts = 0.0;
    bool isSoloistChecked = false;

    ConcertController controller; // TODO elimina
};
