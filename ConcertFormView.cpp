#include "ConcertFormView.h"
#include "musician_form.h"
#include <form.h>
#include <ncurses.h>
#include <cstring>
#include <sstream>

ConcertFormView::ConcertFormView() {}

ConcertFormView::ConcertFormView(const Concert& c) {
    // Inizializza i tuoi campi interni con i dati del concerto esistente
    this->title = c.getTitle();
    this->dates = c.getDates();
    this->places = c.getPlaces();
    // ... altri campi se servono
}

// Helper: rimuove spazi laterali
static std::string trim(const char* buffer) {
    std::string s(buffer);
    size_t start = s.find_first_not_of(" \t");
    size_t end = s.find_last_not_of(" \t");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

// Helper: split da stringa separata da virgole
static std::vector<std::string> split(const std::string& input, char sep = ',') {
    std::stringstream ss(input);
    std::string item;
    std::vector<std::string> result;
    while (std::getline(ss, item, sep)) {
        result.push_back(trim(item.c_str()));
    }
    return result;
}

struct FormComponents {
    FIELD* fields[7];  // 3 input + 3 pulsanti + nullptr
    FORM* form;
};

FormComponents drawForm(const Concert* existing) {
    FormComponents fc;

    const int NUM_FIELDS = 3;
    const int NUM_BUTTONS = 4;

    fc.fields[0] = new_field(1, 40, 2, 30, 0, 0); // Title
    fc.fields[1] = new_field(1, 40, 4, 30, 0, 0); // Places
    fc.fields[2] = new_field(1, 40, 6, 30, 0, 0); // Dates
    fc.fields[3] = new_field(1, 10, 10, 10, 0, 0); // OK
    fc.fields[4] = new_field(3, 12, 10, 20, 0, 0); // CANCEL
    fc.fields[5] = new_field(1, 18, 10, 36, 0, 0); // Add Musician
    fc.fields[6] = new_field(1, 18, 10, 52, 0, 0); // Delete Musician
    fc.fields[7] = nullptr;

    for (int i = 0; i < NUM_FIELDS; ++i) {
        set_field_back(fc.fields[i], A_UNDERLINE);
        field_opts_off(fc.fields[i], O_AUTOSKIP);
    }

    for (int i = NUM_FIELDS; i < NUM_FIELDS + NUM_BUTTONS; ++i) {
        field_opts_off(fc.fields[i], O_EDIT);
        set_field_back(fc.fields[i], COLOR_PAIR(2));
        set_field_fore(fc.fields[i], COLOR_PAIR(2));
    }

    set_field_buffer(fc.fields[3], 0, "[ OK ]");
    set_field_buffer(fc.fields[4], 0, "[ CANCEL ]");
    set_field_buffer(fc.fields[5], 0, "[ Add Musician ]");
    set_field_buffer(fc.fields[6], 0, "[ Delete Musician ]");

    if (existing) {
        set_field_buffer(fc.fields[0], 0, existing->getTitle().c_str());

        std::string placesStr;
        for (const auto& p : existing->getPlaces()) {
            if (!placesStr.empty()) placesStr += ", ";
            placesStr += p;
        }
        set_field_buffer(fc.fields[1], 0, placesStr.c_str());

        std::string datesStr;
        for (const auto& d : existing->getDates()) {
            if (!datesStr.empty()) datesStr += ", ";
            datesStr += d;
        }
        set_field_buffer(fc.fields[2], 0, datesStr.c_str());
    }

    fc.form = new_form(fc.fields);
    post_form(fc.form);

    mvprintw(2, 5, "Title:");
    mvprintw(4, 5, "Places (comma-separated):");
    mvprintw(6, 5, "Dates  (comma-separated):");
    refresh();

    form_driver(fc.form, REQ_FIRST_FIELD);

    return fc;
}

void ConcertFormView::show(const Concert* existing) {
    const int NUM_FIELDS = 3;
    const int NUM_BUTTONS = 4;

    std::string tempTitle, tempPlaces, tempDates;

    if (existing) {
        tempTitle = existing->getTitle();

        const auto& pl = existing->getPlaces();
        for (size_t i = 0; i < pl.size(); ++i) {
            if (i > 0) tempPlaces += ", ";
            tempPlaces += pl[i];
        }

        const auto& dt = existing->getDates();
        for (size_t i = 0; i < dt.size(); ++i) {
            if (i > 0) tempDates += ", ";
            tempDates += dt[i];
        }
    }

redraw:
    clear();
FormComponents fc = drawForm(nullptr);  // Passiamo nullptr perché carichiamo a mano
    FIELD** fields = fc.fields;
    FORM* form = fc.form;

    // Salviamo i dati dell’ultima digitazione
    if (!tempTitle.empty()) set_field_buffer(fields[0], 0, tempTitle.c_str());
    if (!tempPlaces.empty()) set_field_buffer(fields[1], 0, tempPlaces.c_str());
    if (!tempDates.empty()) set_field_buffer(fields[2], 0, tempDates.c_str());

    
    // Reimposta buffer con valori precedenti
    set_field_buffer(fields[0], 0, tempTitle.c_str());
    set_field_buffer(fields[1], 0, tempPlaces.c_str());
    set_field_buffer(fields[2], 0, tempDates.c_str());

    bool done = false;
    int ch;

    while (!done) {
        FIELD* current = current_field(form);
        int currentIndex = field_index(current);

        // Evidenzia il pulsante attivo
        for (int i = NUM_FIELDS; i < NUM_FIELDS + NUM_BUTTONS; ++i) {
            if (i == currentIndex) {
                set_field_back(fields[i], COLOR_PAIR(1));
                set_field_fore(fields[i], COLOR_PAIR(1));
            } else {
                set_field_back(fields[i], COLOR_PAIR(2));
                set_field_fore(fields[i], COLOR_PAIR(2));
            }
        }
        refresh();

        ch = getch();

        switch (ch) {
            case 10: // ENTER
                if (current == fields[3]) { // OK
                    form_driver(form, REQ_VALIDATION);
                    title = trim(field_buffer(fields[0], 0));
                    places = split(field_buffer(fields[1], 0));
                    dates = split(field_buffer(fields[2], 0));
                    done = true;
                } else if (current == fields[4]) { // CANCEL
                    title = "";
                    places.clear();
                    dates.clear();
                    done = true;
                } else if (current == fields[5]) { // Add Musician
                    // Salva temporaneamente i valori correnti
                    form_driver(form, REQ_VALIDATION);
                    tempTitle  = trim(field_buffer(fields[0], 0));
                    tempPlaces = trim(field_buffer(fields[1], 0));
                    tempDates  = trim(field_buffer(fields[2], 0));

                    // ✅ Chiamata immutata come richiesto
                    Musician musician;
                    bool added = runMusicianForm(nullptr, musician);
                    if (added) musicians.push_back(musician);

                    // 🔁 Redraw con ripristino valori
                    unpost_form(form);
                    free_form(form);
                    for (int i = 0; i < NUM_FIELDS + NUM_BUTTONS; ++i)
                        free_field(fields[i]);
                    goto redraw;
                } else if (current == fields[6]) { // Delete Musician
                    std::vector<Musician> musicians = 
                    int choice = runDeleteMusicianForm(musicians);
                } else {
                    form_driver(form, REQ_NEXT_FIELD);
                    form_driver(form, REQ_END_LINE);
                }
                break;

            case KEY_DOWN:
            case 9: // TAB
                form_driver(form, REQ_NEXT_FIELD);
                form_driver(form, REQ_END_LINE);
                break;

            case KEY_UP:
                form_driver(form, REQ_PREV_FIELD);
                form_driver(form, REQ_END_LINE);
                break;

            case KEY_BACKSPACE:
            case 127:
                form_driver(form, REQ_DEL_PREV);
                break;

            default:
                if (currentIndex < NUM_FIELDS)
                    form_driver(form, ch);
                break;
        }
    }

    // Pulizia finale
    unpost_form(form);
    free_form(form);
    for (int i = 0; i < NUM_FIELDS + NUM_BUTTONS; ++i)
        free_field(fields[i]);
}

std::string ConcertFormView::getTitle() const {
    return title;
}

std::vector<std::string> ConcertFormView::getPlaces() const {
    return places;
}

std::vector<std::string> ConcertFormView::getDates() const {
    return dates;
}

std::vector<Musician> ConcertFormView::getMusicians() const {
    return musicians;
}
