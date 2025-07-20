#pragma once

#include "View.h"
#include "MenuBar.h"
#include <form.h>
#define FORM_FIELDS 4

class ConcertInfoForm : public View {
public:
    MenuCommand show() override;
    ~ConcertInfoForm() override = default;
    ConcertInfoForm();
    void setConcert(const Concert* existing);
    void closeForm() override;
    void validateFields() override;

    const std::string getTitle() const;
    const std::string getPlaces() const;
    const std::string getDates() const;
    const std::vector<std::string> getDatesAsVector();
    const std::vector<std::string> getPlacesAsVector();

private:
    const Concert* existing = nullptr;
    std::string title;
    std::string places;
    std::string dates;
    FIELD *fields[FORM_FIELDS];
    FORM *form;
    std::vector<std::string> split(const std::string &input, char sep);
};
