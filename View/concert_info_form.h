#pragma once

#include "View.h"
#include "MenuBar.h"
#include <form.h>

class ConcertInfoForm : public View {
public:
    void show() override;
    MenuCommand getCommand() override;
    ~ConcertInfoForm() override;
    ConcertInfoForm();
    void setConcert(const Concert* existing);

    const std::string getTitle() const;
    const std::string getPlaces() const;
    const std::string getDates() const;
    const std::vector<std::string> getDatesAsVector();
    const std::vector<std::string> getPlacesAsVector();

private:
    static constexpr int NUMBER_OF_FIELDS = 4;
    void init_form() override;
    void closeForm() override;
    void validateFields() override;
    const Concert* existing = nullptr;
    std::string title;
    std::string places;
    std::string dates;
    FIELD *fields[NUMBER_OF_FIELDS];
    FORM *form = nullptr;
    std::vector<std::string> split(const std::string &input, char sep);
};
