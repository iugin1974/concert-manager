#pragma once

#include "View.h"
#include "MenuBar.h"
#include <form.h>

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
    static constexpr int NUMBER_OF_FIELDS = 4;
    const Concert* existing = nullptr;
    std::string title;
    std::string places;
    std::string dates;
    FIELD *fields[NUMBER_OF_FIELDS];
    FORM *form;
    std::vector<std::string> split(const std::string &input, char sep);
};
